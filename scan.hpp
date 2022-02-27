//
//  scan.hpp
//  词法分析器
//
//  Created by Ares wf on 2020/3/29.
//  Copyright © 2020 Ares wf. All rights reserved.
//

#ifndef scan_hpp
#define scan_hpp
#define MAXTOKENLEN 100


#include <stdio.h>
#include "globals.hpp"
#include"util.hpp"
/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];

/* function getToken returns the
 * next token in source file
 
 */

TokenType getToken(void);
int getNextChar();

#endif /* scan_hpp */
