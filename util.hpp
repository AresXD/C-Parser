//
//  util.hpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/29.
//  Copyright © 2020 Ares wf. All rights reserved.
//

#ifndef util_hpp
#define util_hpp

#include <stdio.h>
#include "globals.hpp"

void printToken(TokenType,const char *);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction*/


TreeNode * newNode(NodeKind kind);


char * copyString( char *);

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode *);
string OpeLookUp(TokenType tk);
string NodetoString(NodeKind nk);

void printToken(TokenType token, const char* tokenString ){
    switch (token) {
        case ELSE:
        case IF:
        case INT:
        case RETURN:
        case VOID:
        case WHILE:
            fprintf(listing,
                    "reserved word: %s\n",tokenString);
            break;
        case PLUS: fprintf(listing,":+\n"); break;
        case MINUS: fprintf(listing,"-\n"); break;
        case MULTIPLY: fprintf(listing,"*\n"); break;
        case DIVIDE: fprintf(listing,"/\n"); break;
        case LT: fprintf(listing,"<\n"); break;
        case NGT: fprintf(listing,"<=\n"); break;
        case GT: fprintf(listing,">\n"); break;
        case NLT: fprintf(listing,">=\n"); break;
        case EQV: fprintf(listing,"==\n"); break;
        case NEQV: fprintf(listing,"!=\n"); break;
        case EQ: fprintf(listing,"=\n"); break;
        case SEMI: fprintf(listing,";\n"); break;
        case COMMA: fprintf(listing,",\n"); break;
        case LPARENTHESES: fprintf(listing,"(\n"); break;
        case RPARENTHESES: fprintf(listing,")\n"); break;
        case LBRACKETS: fprintf(listing,"[\n"); break;
        case RBRACKETS: fprintf(listing,"]\n"); break;
        case LBRACE: fprintf(listing,"{\n"); break;
        case RBRACE: fprintf(listing,"}\n"); break;
        case LANNOTATION:fprintf(listing,"/*\n");break;
        case RANNOTATION:fprintf(listing,"*/\n");break;
        case NUM:
            fprintf(listing,
                    "NUM, val= %s\n",tokenString);
            break;
        case ID:
            fprintf(listing,
                    "ID, name= %s\n",tokenString);
            break;
        case ERROR:
            fprintf(listing,
                    "ERROR: %s\n",tokenString);
            break;
        case ENDFILE:
            fprintf(listing,
                    "EOF\n");
            
            break;
        default: /* should never happen */
            fprintf(listing,"Unknown token: %d\n",token);
    }
}



/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
    char * t;
    if (s==NULL) return NULL;
    n = strlen(s)+1;
    t = (char *)malloc(n);
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else strcpy(t,s);
    return t;
}
TreeNode * newNode(NodeKind kind){
    TreeNode *t=(TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t==NULL){
        cout<<"create error\n";
        
    }
    else{
        for(i=0;i<4;i++){
            t->child[i]=NULL;
            
        }
        t->sibling=NULL;
        t->nodekind=kind;
        t->lineno=lineno;
        if(kind==Opk||kind==Intk||kind==Idk){
            if(kind==Idk)
                t->attr.name="";
            
        }
        if(kind==Constk)
            t->attr.val=0;
    }
    return t;
    
}
string OpeLookUp(TokenType tk){
    int i;
    for (i=0;i<11; i++) {
        if(tk==ope[i].tk){
            return ope[i].str;
        }
    }
    
    return "error type";
    
    
}
string NodetoString(NodeKind nk){
    int i;
    for (i=0; i<20;i++) {
        if (nk==nodekind[i].nk) {
            return nodekind[i].str;
            break;
        }
    }
    return "Error Type";
}


#endif /* util_hpp */
