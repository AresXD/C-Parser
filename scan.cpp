//
//  scan.cpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/29.
//  Copyright © 2020 Ares wf. All rights reserved.
//
//PLUS,MINUS,MULTIPLY,DIVIDE,LT,NGT,GT,NLT,EQV,NEQV,EQ,SEMI,COMMA,LPARENTHESES,RPARENTHESES,LBRACKETS,RBRACKETS,LBRACE,RBRACE,LANNOTATION,RANNOTATION
#include "scan.hpp"

typedef enum {
    START,INNLT,INNGT,INNEQV,INEQV,INNUM,INPREAN,INAN,INAFTERAN,INID,DONE
    
}StateType;
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
#define BUFLEN 1024

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
 from lineBuf, reading in a new line if lineBuf is exhausted*/
int getNextChar(){
    
    if(!(linepos<bufsize)){
        lineno++;
        if (fgets(lineBuf,BUFLEN-1,source)) {
            bufsize=strlen(lineBuf);
            linepos=0;
            return lineBuf[linepos++];
        }
        else
        {
            EOF_flag=true;
            return EOF;
        }
    }
    else return lineBuf[linepos++];
}
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* lookup table of reserved words */
static struct
{ char const * str;
    TokenType tok;
} reservedWords[MAXRESERVED]
= {{"if",IF},{"else",ELSE},{"return",RETURN},{"int",INT},
    {"while",WHILE},{"void",VOID}};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup (char * s)
{ int i;
    for (i=0;i<MAXRESERVED;i++)
        if (!strcmp(s,reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
    int tokenStringIndex = 0;
    /* holds current token to be returned */
    TokenType currentToken;
    /* current state - always begins at START */
    StateType state = START;
    /* flag to indicate save to tokenString */
    int save;
    while (state != DONE)
    { int c = getNextChar();
        save = TRUE;
        switch (state)
        { case START:
                if (isdigit(c))
                    state = INNUM;
                else if (isalpha(c))
                    state = INID;
                else if (c == '>')
                    state = INNLT;
                else if (c == '<')
                    state = INNGT;
                else if (c == '=')
                    state = INEQV;
                else if (c == '!')
                    state = INNEQV;
                else if (c == '/')//注释
                {
                    save=FALSE;
                    state = INPREAN;
                }
            
                else if ((c == ' ') || (c == '\t') || (c == '\n'))
                    save = FALSE;
                else
                { state = DONE;
                    switch (c)
                    { case EOF:
                            save = FALSE;
                            currentToken = ENDFILE;
                            break;
                        /*case '=':
                            currentToken = EQ;
                            break;
                        case '<':
                            currentToken = LT;
                            break;*/
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = MULTIPLY;
                            break;
                        /*case '/':
                            currentToken = DIVIDE;
                            break;*/
                        case '(':
                            currentToken = LPARENTHESES;
                            break;
                        case ')':
                            currentToken = RPARENTHESES;
                            break;
                        case '[':
                            currentToken = LBRACKETS;
                            break;
                        case ']':
                            currentToken = RBRACKETS;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        case ',':
                            currentToken = COMMA;
                            break;
                        case '{':
                            currentToken = LBRACE;
                            break;
                        case '}':
                            currentToken = RBRACE;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;
            case INNLT:
                state = DONE;
                if (c == '=')
                    currentToken = NLT;
                else
                { /* backup in the input */
                    ungetNextChar();
                    save = FALSE;
                    currentToken=GT;
                }
                break;
            case INNGT:
                state = DONE;
                if (c == '=')
                    currentToken = NGT;
                else
                { /* backup in the input */
                    ungetNextChar();
                    save = FALSE;
                    currentToken = LT;
                }
                break;
            case INEQV:
                state=DONE;
                if (c=='=') {
                currentToken=EQV;
                }
                else{
                    ungetNextChar();
                    save=FALSE;
                    currentToken = EQ;
                }
                break;
            
            case INNEQV:
                state=DONE;
                if (c=='=') {
                    currentToken=EQV;
                }
                else{
                    ungetNextChar();
                    save=FALSE;
                    currentToken = ERROR;
                }
                break;
            case INNUM:
                if (!isdigit(c))
                { /* backup in the input */
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = NUM;
                }
                break;
            case INID:
                if (!isalpha(c))
                { /* backup in the input */
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = ID;
                }
                break;
            case INAN://annotation
                save=FALSE;
                if (c==EOF) {
                    state=DONE;
                    currentToken=ENDFILE;
                }
               /* else if ((c == ' ') || (c == '\t') || (c == '\n'))
                {   save = FALSE;
                    state=INAN;
                }*/
                else if(c=='*'){
                    state=INAFTERAN;
                        }
                else {
                    state=INAN;
                }
                break;
            case INPREAN:
                if (c=='*') {
                    state=INAN;
                    save=false;
                }
                else
                {
                    ungetNextChar();
                    currentToken=DIVIDE;
                    state=DONE;
                }
                break;
                
            case INAFTERAN:
                save=FALSE;
                if(c=='/'){
                    state=START;
                }
                
                else if (c=='*')
                {
                    state=INAFTERAN;
                }
                else
                    state=INAN;
                break;
                
            case DONE:
            default: /* should never happen */
                fprintf(listing,"Scanner Bug: state= %d\n",state);
                state = DONE;
                currentToken = ERROR;
                break;
        }
        if ((save) && (tokenStringIndex <= MAXTOKENLEN))
            tokenString[tokenStringIndex++] = (char) c;
        if (state == DONE)
        { tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (TraceScan) {
        fprintf(listing,"\t%d: ",lineno);
        printToken(currentToken,tokenString);
    }
    return currentToken;
} /* end getToken */

 

