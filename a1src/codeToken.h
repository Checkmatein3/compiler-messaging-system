/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __CODETOKEN_H__
#define __CODETOKEN_H__

typedef enum CodeTokenTypeEnum {
    identifier,
    keyword,
    special,
    comment,
    multilineComment,
    stringLiteral
}CodeTokenType;

/*
    
    Pre:    
    Post:   
*/
int isCPPLKeyword(char* token);

#endif