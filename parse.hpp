
//  parse.hpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/30.
//  Copyright © 2020 Ares wf. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp
#include "globals.hpp"
#include "util.hpp"
#include "scan.hpp"

using namespace std;

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
//static TreeNode * arg_list(void);
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
//static TreeNode * arg_list(void);

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
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

#endif /* parse_hpp */
