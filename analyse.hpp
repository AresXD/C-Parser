//
//  analyse.hpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/30.
//  Copyright © 2020 Ares wf. All rights reserved.
//

#ifndef analyse_hpp
#define analyse_hpp

using namespace std;
typedef enum
{
    ENDFILE,ERROR,
    //reserved words
    ELSE,IF,INT,RETURN,VOID,WHILE,
    // multicharacter tokens
    ID,NUM,
    // special symbols + - * / < <= > >= == != = ; , () [] {}
    PLUS,MINUS,MULTIPLY,DIVIDE,LT,NGT,GT,NLT,EQV,NEQV,EQ,SEMI,COMMA,LPARENTHESES,RPARENTHESES,LBRACKETS,RBRACKETS,LBRACE,RBRACE,LANNOTATION,RANNOTATION
}TokenType;
FILE* source; //*source code text file
FILE* listing; // listing output text file
FILE* code; //code text file for TM simulator

int lineno; //source line number for listing

string output;



typedef enum {Funck,Intk,Idk,Paramsk,Compk,Constk,Callk,ArgsK,Voidk,Var_Declk,Array_Declk,Array_Elemk,Assignk,Opk,Selection_Stmtk,Iteration_Stmtk,Return_Stmtk,IfK,WhileK,ElseK,ReturnK} NodeKind;
struct{
    string str;
    NodeKind nk;
}nodekind[20]={
    {"Funck",Funck},{"Intk",Intk},{"Paramsk",Paramsk},{"Compk",Compk},{"Constk",Constk},{"Callk",Callk},{"ArgsK",ArgsK},{"Voidk",Voidk},{"Var_Declk",Var_Declk},{"Array_Declk",Array_Declk},{"Array_Elemk",Array_Elemk},{"Assignk",Assignk},{"Opk",Opk},{"Selection_Stmtk",Selection_Stmtk},{"Iteration_Stmtk",Iteration_Stmtk},{"Return_Stmtk",Return_Stmtk},{"IfK",IfK},{"WhileK",WhileK},{"ElseK",ElseK},{"ReturnK",ReturnK}
};
struct {
    string str;
    TokenType tk;
}ope[11]={
    {"+",PLUS},{"-",MINUS},{"*",MULTIPLY},{"/",DIVIDE},{"<",LT},{"<=",NGT},{">",GT},{">=",NLT},{"==",EQV},{"!=",NEQV},{"=",EQ}
    
};


#define MAXCHILDREN 4

typedef struct treeNode
{ struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;
    union { TokenType op;
        int val;
        string name; } attr;
    
} TreeNode;

int EchoSource = FALSE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;


typedef enum {
    START,INNLT,INNGT,INNEQV,INEQV,INNUM,INPREAN,INAN,INAFTERAN,INID,DONE
    
}StateType;
#define MAXTOKENLEN 100
//lexeme of identifier or reserved word
char tokenString[MAXTOKENLEN+1];
#define BUFLEN 1024

static char lineBuf[BUFLEN]; //holds the current line
static int linepos = 0; //current position in LineBuf
static int bufsize = 0; // current size of buffer string
static int EOF_flag = FALSE; //corrects ungetNextChar behavior on EOF

void printToken(TokenType,const char *);

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

// lookup table of reserved words
static struct
{ char const * str;
    TokenType tok;
} reservedWords[MAXRESERVED]
= {{"if",IF},{"else",ELSE},{"return",RETURN},{"int",INT},
    {"while",WHILE},{"void",VOID}};

//lookup an identifier to see if it is a reserved word
// uses linear search
static TokenType reservedLookup (char * s)
{ int i;
    for (i=0;i<MAXRESERVED;i++)
        if (!strcmp(s,reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}


TokenType getToken(void)
{
    int tokenStringIndex = 0;
    
    TokenType currentToken;
    
    StateType state = START;
    
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
                            
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = MULTIPLY;
                            break;
                            
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
                {
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
                {
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
                {
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = NUM;
                }
                break;
            case INID:
                if (!isalpha(c))
                {
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
            default:
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
}


TreeNode * newNode(NodeKind kind);


char * copyString( char *);


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
        default:
            fprintf(listing,"Unknown token: %d\n",token);
    }
}



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

TokenType token;

TreeNode * declaration(void);

TreeNode * declaration_list(void);

TreeNode * params(void);
TreeNode * param_list(TreeNode *k);
TreeNode * param(TreeNode *k);
TreeNode * compound_stmt(void);
TreeNode * local_declarations(void);
TreeNode * statement_list(void);

TreeNode * statement(void);
TreeNode * expression_stmt (void);
TreeNode * selection_stmt (void);

TreeNode * iteration_stmt(void);
TreeNode * return_stmt (void);
TreeNode * expression(void);
TreeNode * var(void);
TreeNode * simple_expression(TreeNode *k);
TreeNode * additive_expression (TreeNode *k);
TreeNode * term(TreeNode *k);
TreeNode * factor(TreeNode *k);
TreeNode * call (TreeNode *k);
TreeNode * args(void);

void syntaxError(string * message);



void syntaxError(string * message)
{ fprintf(listing,"\n>>> ");
    fprintf(listing,"Syntax error at line %d:",lineno);
    cout<<message;
    Error = TRUE;
}

void match(TokenType expected)
{ if (token == expected) token = getToken();
else {
    cout<<"unexpected token type"<<endl;
    printToken(token,tokenString);
    fprintf(listing,"      ");
}
}


TreeNode * declaration_list(void){//declaration_list->declaration{declaration}
    TreeNode *t=declaration();
    TreeNode *p=t;
    while ((token!=INT)&&(token!=VOID)&&(token!=ENDFILE)) {
        token=getToken();
        if(token==ENDFILE)
            break;
    }
    while (token==INT||token==VOID) {
        TreeNode *q;
        q=declaration();
        if(q!=NULL){
            if(t==NULL){
                t=p=q;
            }
            else{
                p->sibling=q;
                p=q;
            }
        }
        
    }
    match(ENDFILE);
    return t;
}
TreeNode * declaration(void){
    TreeNode *t=NULL;
    TreeNode *p=NULL;
    TreeNode *q=NULL;
    TreeNode *s=NULL;
    TreeNode *a=NULL;
    if (token==INT) {
        p=newNode(Intk);
        match(INT);
    }
    else{
        p=newNode(Voidk);
        match(VOID);
    }
    if(p!=NULL&&token==ID){
        q=newNode(Idk);
        q->attr.name=copyString(tokenString);
        match(ID);
        if(token==LPARENTHESES){
            t=newNode(Funck);
            t->child[0]=p;
            t->child[1]=q;
            match(LPARENTHESES);
            t->child[2]=params();
            match(RPARENTHESES);
            t->child[3]=compound_stmt();
            
        }
        else if (token==LBRACKETS){
            t=newNode(Var_Declk);
            a=newNode(Array_Declk);
            t->child[0]=p;
            t->child[1]=a;
            match(LBRACKETS);
            s=newNode(Constk);
            s->attr.val=atoi(tokenString);
            match(NUM);
            a->child[0]=q;
            a->child[1]=s;
            match(RBRACKETS);
            match(SEMI);
            
            
        }
        else if (token==SEMI){
            t=newNode(Var_Declk);
            t->child[0]=p;
            t->child[1]=q;
            match(SEMI);
        }
        
    }
    return t;
}
TreeNode * params(void){//params->params_list|void
    
    TreeNode *t=newNode(Paramsk);
    TreeNode *p=NULL;
    if(token==VOID){
        p=newNode(Voidk);
        match(VOID);
        if(token==RPARENTHESES){
            if(t!=NULL){
                t->child[0]=p;
            }
        }
        else{
            t->child[0]=param_list(p);
        }
        
    }
    else{
        t->child[0]=param_list(p);
    }
    return t;
    
}
TreeNode * param_list(TreeNode *k){
    TreeNode*t=param(k);
    TreeNode *p=t;
    k=NULL;
    while (token==COMMA) {
        TreeNode *q=NULL;
        match(COMMA);
        q=param(k);
        if (q!=NULL) {
            if(t==NULL)
            {
                t=p=q;
            }
            else{
                p->sibling=q;
                p=q;
            }
        }
        
    }
    return t;
}
TreeNode * param(TreeNode *k){
    TreeNode *t=newNode(Paramsk);
    TreeNode *p=NULL;
    TreeNode *q=NULL;
    if(k==NULL&&token==VOID){
        p=newNode(Voidk);
        match(VOID);
        
    }
    else if (k==NULL&&token==INT){
        p=newNode(Intk);
        match(INT);
    }
    else if (k!=NULL){
        p=k;
    }
    if(p!=NULL){
        t->child[0]=p;
        if (token==ID) {
            q=newNode(Idk);
            q->attr.name=copyString(tokenString);
            t->child[1]=q;
            match(ID);
            
        }
        if (token==LBRACKETS&&(t->child[1]!=NULL)) {
            match(LBRACKETS);
            t->child[2]=newNode(Idk);
            match(RBRACKETS);
        } else {
            return t;
        }
    }
    return t;
}
TreeNode * compound_stmt(void){
    TreeNode *t=newNode(Compk);
    match(LBRACE);
    t->child[0]=local_declarations();
    t->child[1]=statement_list();
    match(RBRACE);
    return t;
    
}
TreeNode * local_declarations(void){
    TreeNode *t=NULL;
    TreeNode *q=NULL;
    TreeNode *p=NULL;
    while (token==INT||token==VOID) {
        p=newNode(Var_Declk);
        if(token==INT){
            TreeNode *q1=newNode(Intk);
            p->child[0]=q1;
            match(INT);
        }
        else{
            TreeNode *q1=newNode(Voidk);
            p->child[0]=q1;
            match(VOID);
            
        }
        if(p!=NULL&&token==ID){
            TreeNode *q2=newNode(Idk);
            q2->attr.name=copyString(tokenString);
            p->child[2]=q2;
            match(ID);
            if (token==LBRACKETS) {
                TreeNode *q3=newNode(Var_Declk);
                p->child[3]=q3;
                match(LBRACKETS);
                match(RBRACKETS);
                match(SEMI);
            }
            else{
                match(SEMI);
            }
            
        }
    }
    if (p!=NULL) {
        if(t==NULL){
            t=p=q;
            
        }
        else{
            q->sibling=p;
            q=p;
        }
    }
    return t;
}
TreeNode * statement_list(void){//statement_list->statement{statement}
    TreeNode *t=statement();
    TreeNode *p=NULL;
    while (token==IF||token==LBRACKETS||token==ID||token==WHILE||token==RETURN||token==SEMI||token==LPARENTHESES||token==NUM) {
        TreeNode *q=statement();
        if(q!=NULL){
            if(t==NULL){
                t=p=q;
                
            }
            else{
                p->sibling=q;
                p=q;
            }
        }
    }
    return t;
}

TreeNode * statement(void){
    TreeNode *t=NULL;
    switch (token) {
        case IF:
            t=selection_stmt();
            break;
        case LBRACE:
            t=compound_stmt();
            break;
        case WHILE:
            t=iteration_stmt();
            break;
        case RETURN:
            t=return_stmt();
            break;
        case ID:
        case LPARENTHESES:
        case NUM:
        case SEMI:
            t=expression_stmt();
            break;
            
        default:
            token=getToken();
            break;
    }
    return t;
}
//static TreeNode * expression_stmt (void);
TreeNode * selection_stmt (void){
    TreeNode *t=newNode(Selection_Stmtk);
    match(IF);
    match(LPARENTHESES);
    if(t!=NULL){
        t->child[0]=expression();
        
    }
    match(RPARENTHESES);
    t->child[1]=statement();
    if(token==ELSE){
        match(ELSE);
        t->child[2]=statement();
    }
    return t;
    
    
    
    
}

TreeNode * iteration_stmt(void){
    TreeNode *t=newNode(Iteration_Stmtk);
    match(WHILE);
    match(LPARENTHESES);
    if(t!=NULL)
        t->child[0]=expression();
    match(RPARENTHESES);
    if(t!=NULL)
        t->child[1]=statement();
    return t;
}
TreeNode * return_stmt (void){
    TreeNode *t=newNode(Return_Stmtk);
    match(RETURN);
    if(token==SEMI)
        match(SEMI);
    else if (t!=NULL){
        t->child[0]=expression();
        match(SEMI);
    }
    return t;
}
TreeNode * expression_stmt(void){
    TreeNode  *t=NULL;
    if(token==SEMI){
        match(SEMI);
        return t;
    }
    else{
        t=expression();
        match(SEMI);
    }
    return t;
}
TreeNode * expression(void){
    TreeNode *t=var();
    if(t==NULL){
        t=simple_expression(t);
    }
    else{
        TreeNode *p=NULL;
        if(token==EQ){
            p=newNode(Assignk);
            p->child[0]=t;
            p->child[1]=expression();
            match(EQ);
            return p;
        }
        else{
            t=simple_expression(t);
        }
    }
    return t;
}
TreeNode * var(void){
    TreeNode *t=NULL;
    TreeNode *p=NULL;
    TreeNode *q=NULL;
    if(token==ID){
        p->attr.name=copyString(tokenString);
        match(ID);
        if(token==LBRACKETS){
            match(LBRACKETS);
            q=expression();
            t=newNode(Array_Elemk);
            t->child[0]=p;
            t->child[1]=q;
        }
        else t=p;
        
    }
    return t;
}
TreeNode * simple_expression(TreeNode *k){
    TreeNode *t=additive_expression(k);
    k=NULL;
    if(token==NGT||token==LT||token==GT||token==NLT||token==EQV||token==NEQV){
        TreeNode *q=newNode(Opk);
        q->attr.op=token;
        q->child[0]=t;
        t=q;
        match(token);
        t->child[1]=additive_expression(k);
        return t;
    }
    return t;
}
TreeNode * additive_expression (TreeNode *k){
    TreeNode *t=term(k);
    k=NULL;
    while (token==PLUS||token==MINUS) {
        TreeNode *q=newNode(Opk);
        q->attr.op=token;
        q->child[0]=t;
        match(token);
        q->child[1]=term(k);
        t=q;
        
    }
    return t;
}
TreeNode * term(TreeNode *k){
    TreeNode *t=factor(k);
    k=NULL;
    while (token==DIVIDE||token==MULTIPLY) {
        TreeNode *q=newNode(Opk);
        q->attr.op=token;
        q->child[0]=t;
        match(token);
        q->child[1]=factor(k);
        t=q;
    }
    return t;
}
TreeNode * factor(TreeNode *k){
    TreeNode *t=NULL;
    if(k!=NULL){
        if(token==LPARENTHESES&&k->nodekind!=Array_Elemk){
            t=call(k);
            
        }
        else
            t=k;
    }
    else{
        switch (token) {
            case ID:
                k=var();
                if(token==LPARENTHESES&&k->nodekind!=Array_Elemk){
                    t=call(k);
                }
                break;
            case LPARENTHESES:
                match(LPARENTHESES);
                t=expression();
                match(RPARENTHESES);
                break;
            case NUM:
                t=newNode(Constk);
                if(t!=NULL&&token==NUM){
                    t->attr.val=atoi(tokenString);
                    
                }
                match(NUM);
                break;
                
                
                
            default:
                token=getToken();
                break;
        }
    }
    return t;
}
TreeNode * call (TreeNode *k){
    TreeNode *t=newNode(Callk);
    if(k!=NULL){
        t->child[0]=k;
        match(LPARENTHESES);
        if (token==RPARENTHESES) {
            match(RPARENTHESES);
            return t;
        }
        else if(k!=NULL){
            t->child[1]=args();
            match(RPARENTHESES);
        }
    }
    return t;
}
TreeNode * args(void){
    TreeNode *t=newNode(ArgsK);
    TreeNode *p=NULL;
    TreeNode *s=NULL;
    if(token!=RPARENTHESES) {
        
        s=expression();
        p=s;
        while (token==COMMA) {
            TreeNode *q;
            match(COMMA);
            if(q!=NULL){
                if(s==NULL){
                    s=p=q;
                }
                else{
                    p->sibling=q;
                    p=q;
                }
            }
        }
    }
    if (s!=NULL) {
        t->child[0]=s;
    }
    return t;
    
}

int blank_number=0;
void PreOrder(TreeNode *t){
    string blank="  ";
    int i;
    for (i=0; i<blank_number; i++) {
        blank+=" ";
    }
    if(t!=NULL){
        if(t->nodekind==Opk){
            cout<<blank<<"Op: "<<OpeLookUp(t->attr.op)<<endl;
            output+=blank+"Op: "+OpeLookUp(t->attr.op)+"\n";
            
            
        }
        else if (t->nodekind==Idk){
            cout<<blank<<NodetoString(t->nodekind)<<": "<<t->attr.name<<endl;
            output+=blank+NodetoString(t->nodekind)+": "+t->attr.name+"\n";
            
        }
        else if (t->nodekind==Constk){
            cout<<blank<<NodetoString(t->nodekind)<<": "<<t->attr.val<<endl;
            int n=t->attr.val;
            strstream ss;
            string s;
            ss<<n;
            ss>>s;
            output+=blank+NodetoString(t->nodekind)+": "+s+"\n";
            
            
            
        }
        else if (t->nodekind==Assignk){
            cout<<blank<<"Assign"<<endl;
            output+=blank+"Assign"+"\n";
        }
        else if (t->nodekind==Selection_Stmtk){
            cout<<blank<<"If"<<endl;
            output+=blank+"If"+"\n";
        }
        else if (t->nodekind==Iteration_Stmtk){
            cout<<blank<<"While"<<endl;
            output+=blank+"While"+"\n";
        }
        else if (t->nodekind==Return_Stmtk){
            cout<<blank<<"Return"<<endl;
            output+=blank+"Return"+"\n";
        }
        else{
            cout<<blank<<NodetoString(t->nodekind)<<endl;
            output+=blank+NodetoString(t->nodekind)+"\n";
        }
    }
    for (i=0; i<MAXCHILDREN; i++) {
        if(t->child[i]!=NULL){
            blank_number+=2;
            PreOrder(t->child[i]);
            blank_number-=2;
        }
    }
    if (t->sibling!=NULL) {
        PreOrder(t->sibling);
    }
}
void parse(void){
    TreeNode *t;
    cout<<"Syntax Tree:\n";
    token=getToken();
    t=declaration_list();
    
}

#endif /* analyse_hpp */
