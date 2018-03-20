/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include <stdio.h>
#include "twoDArray.h"
#include "codeToken.h"

typedef enum ConvertStateEnum {
    globalScope,
    functionScope,
    blockScope
}ConvertState;


/*
    Converts c++light(.cc) source code to c(.c) source code.
    Pre:    cPPLFilePath = c++Light file path
    Post:   the converted source code is in a .c file with the same name as the original
*/
int convertCPPLToC(TwoDArray** codeTokens, TwoDArray** followingWhitespace, 
    CodeTokenType** codeTokenTypes, int** scopingLevels, char* outputFilePath);

/*
    
    Pre:    
    Post:   
*/
void dumpFuncPtrDec(FILE* outputFile, char* className, char* funcName, 
    TwoDArray* funcCallTokens, TwoDArray* funcCallWhitespace, 
    TwoDArray* funcParamTypes);

/*
    
    Pre:    
    Post:   
*/
char* createFuncPtrId(char* className, char* funcName, TwoDArray* funcParamTypes);

/*
    
    Pre:    
    Post:   
*/
int dumpToFile(FILE* outputFile, int startToken, int endToken, 
    TwoDArray* codeTokens, TwoDArray* followingWhitespace);

/*
void getGlobalVars(int startToken, int endToken, int scopingLevel,
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes, int* scopingLevels) {
*/

/*
    
    Pre:    
    Post:   
*/
int findScopingLevel(int startToken, int endToken, 
    int* scopingLevels, int scopingLevel);

/*
    
    Pre:    
    Post:   
*/
TwoDArray* getKeywords(int startBracket, int endBracket,
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int dumpMethodDefs(FILE* outputFile, int startPos, int endPos, 
    TwoDArray* codeTokens, TwoDArray* followingWhitespace, 
    CodeTokenType* codeTokenTypes, int* scopingLevels);

/*
    
    Pre:    
    Post:   
*/
TwoDArray* getVars(int startBracket, int endBracket,
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
void dumpStructConstructorCall(FILE* outputFile, const char* objDecId, const char* className);

/*
    
    Pre:    
    Post:   
*/
void dumpStructConstructorDef(FILE* outputFile, const char* className, 
    TwoDArray* funcPtrs, int scopingLevel);

/*
    
    Pre:    
    Post:   
*/
int findNextFuncDef(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes, int* firstTypePos);

/*
    
    Pre:    
    Post:   
*/
int isFuncDef(int tokenPos, TwoDArray* codeTokens, 
    CodeTokenType* codeTokenTypes, int* firstTypePos);

/*
    
    Pre:    
    Post:   
*/
int findMain(TwoDArray* codeTokens);

/*
    
    Pre:    
    Post:   
*/
int isMain(int tokenPos, TwoDArray* codeTokens);

/*
    
    Pre:    
    Post:   
*/
int findNextClassDef(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int findNextTokenType(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes, 
    CodeTokenType codeTokenType);

/*
    
    Pre:    
    Post:   
*/
int findNextObjDec(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int isObjDec(int objIdPos, TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int isFuncPtrCall(int tokenPos, TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int findNextVarDec(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int findNextVarDef(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int findNextClassDec(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes);

/*
    
    Pre:    
    Post:   
*/
int findNext(int startToken, int endToken, 
    TwoDArray* codeTokens, const char* tokenToFind);

/*
    
    Pre:    
    Post:   
*/
int findPrev(int startToken, int endToken, 
    TwoDArray* codeTokens, const char* tokenToFind);

#endif