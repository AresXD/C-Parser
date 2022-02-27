//
//  globals.hpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/26.
//  Copyright © 2020 Ares wf. All rights reserved.
//

#ifndef globals_hpp
#define globals_hpp

#include <iostream>
#include <strstream>
#include <cstdlib>
#include <ctype.h>
#include <string.h>
#include <cstring>
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6
using namespace std;
typedef enum
{
    ENDFILE,ERROR,
    /*reserved words*/
    ELSE,IF,INT,RETURN,VOID,WHILE,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols + - * / < <= > >= == != = ; , () [] {} */
  PLUS,MINUS,MULTIPLY,DIVIDE,LT,NGT,GT,NLT,EQV,NEQV,EQ,SEMI,COMMA,LPARENTHESES,RPARENTHESES,LBRACKETS,RBRACKETS,LBRACE,RBRACE,LANNOTATION,RANNOTATION
}TokenType;
extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

string output;

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {Funck,Intk,Idk,Paramsk,Compk,Constk,Callk,ArgsK,Voidk,Var_Declk,Array_Declk,Array_Elemk,Assignk/*while*/,Opk,Selection_Stmtk,Iteration_Stmtk,Return_Stmtk,IfK,WhileK,ElseK,ReturnK} NodeKind;
struct{
    string str;
    NodeKind nk;
}nodekind[20]={
    {"Funck",Funck},{"Intk",Intk},{"Paramsk",Paramsk},{"Compk",Compk},{"Constk",Constk},{"Callk",Callk},{"ArgsK",ArgsK},{"Voidk",Voidk},{"Var_Declk",Var_Declk},{"Array_Declk",Array_Declk},{"Array_Elemk",Array_Elemk},{"Assignk",Assignk}/*while*/,{"Opk",Opk},{"Selection_Stmtk",Selection_Stmtk},{"Iteration_Stmtk",Iteration_Stmtk},{"Return_Stmtk",Return_Stmtk},{"IfK",IfK},{"WhileK",WhileK},{"ElseK",ElseK},{"ReturnK",ReturnK}
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
         /* for type checking of exps */
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif /* globals_hpp */
