//
//  main.cpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/26.
//  Copyright © 2020 Ares wf. All rights reserved.
//

#include <iostream>
#include "globals.hpp"
#include <stdio.h>

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE TRUE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

//#include "util.hpp"
#include "scan.hpp"
//#include "parse.hpp"
//#include "analyse.hpp"
//#include "cgen.hpp"

using namespace std;
/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = TRUE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;
int main( int argc, char * argv[] )
{ //TreeNode * syntaxTree;
  
    source = fopen("test.txt","r");
    if (source==NULL)
    { fprintf(stderr,"File not found\n");
        exit(1);
    }
    listing = stdout;  //send listing to screen
    fprintf(listing,"\nC- COMPILATION: \n");
#if NO_PARSE
    while (getToken()!=ENDFILE);
#else
    syntaxTree = parse();
    if (TraceParse) {
        fprintf(listing,"\nSyntax tree:\n");
        printTree(syntaxTree);
    }
#if !NO_ANALYZE
    if (! Error)
    { if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
        buildSymtab(syntaxTree);
        if (TraceAnalyze) fprintf(listing,"\nChecking Types...\n");
        typeCheck(syntaxTree);
        if (TraceAnalyze) fprintf(listing,"\nType Checking Finished\n");
    }
#if !NO_CODE
    if (! Error)
    { char * codefile;
        int fnlen = strcspn(pgm,".");
        codefile = (char *) calloc(fnlen+4, sizeof(char));
        strncpy(codefile,pgm,fnlen);
        strcat(codefile,".tm");
        code = fopen(codefile,"w");
        if (code == NULL)
        { printf("Unable to open %s\n",codefile);
            exit(1);
        }
        codeGen(syntaxTree,codefile);
        fclose(code);
    }
#endif
#endif
#endif
    fclose(source);
    return 0;
}

/*int main(){
    FILE *f=fopen("new.txt","w+");
    if(f==NULL){
        printf("no such file");
    }
    string a;
    std::cin>>a;
    return 0;
    
}*/

