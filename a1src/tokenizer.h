/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#define MAX_STRING_SIZE 128
#define DEFAULT_LENGTH 128
#include "twoDArray.h"
#include "codeToken.h"

typedef enum BuildStateEnum {
    buildingToken,
    singleChar,
    buildingArrow,
    buildingCodeFragment,
    buildingNumber,
    buildingWhitespace,
    buildingComment,
    buildingMultilineComment,
    buildingStringLiteral
}BuildState;

/*
    Tokenizes c++light(.cc) source code 
    Pre:    cPPLFilePath = c++Light file path
    Post:   the codeTokens, followingWhitespace, and codeTokenTypes are populated
*/
int tokenizeCPPL(char* cPPLFilePath, TwoDArray** codeTokens,
    TwoDArray** followingWhitespace, CodeTokenType** codeTokenTypes, int** scopingLevels);

/*
    Processes a source code character based on the current build state
    Pre:    codeChar = the source code character
            buildState = the current build state
    Post:   the new build state
*/
BuildState processCodeChar(char codeChar, char previousChar, 
    BuildState buildState, int* scopingLevel);

/*
    Processes a source code character for the token build state    
    Pre:    codeChar = the source code character
    Post:   the new build state
*/
BuildState processCodeCharToken(char codeChar, char previousChar, int* scopingLevel);

/*
    Processes a source code character for the string literal build state    
    Pre:    codeChar = the source code character
    Post:   the new build state
*/
BuildState processCodeCharStringLiteral(char codeChar, char previousChar);

/*
    Processes a source code character for the string literal build state    
    Pre:    codeChar = the source code character
    Post:   the new build state
*/
BuildState processCodeCharComment(char codeChar);

/*
    Processes a source code character for the string literal build state    
    Pre:    codeChar = the source code character
    Post:   the new build state
*/
BuildState processCodeCharMultilineComment(char codeChar, char previousChar);

/*
    
    Pre:    
    Post:   
*/
CodeTokenType getCorrespondingTokenType(BuildState lastNonWhitespace, char* token);

#endif