/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "twoDArray.h"
#include "tokenizer.h"
#include "codeToken.h"

int tokenizeCPPL(char* cPPLFilePath, TwoDArray** codeTokens,
    TwoDArray** followingWhitespace, CodeTokenType** codeTokenTypes, int** scopingLevels) {

    /*initialization for tokenizing*/
    *codeTokens = initTwoDArray(DEFAULT_LENGTH);
    *followingWhitespace = initTwoDArray(DEFAULT_LENGTH);
    *codeTokenTypes = malloc(sizeof(CodeTokenType) * DEFAULT_LENGTH);
    *scopingLevels = malloc(sizeof(int) * DEFAULT_LENGTH);
    
    FILE* cPPLFile;
    cPPLFile = fopen(cPPLFilePath, "r");
    if(cPPLFile == NULL) {
        printf("Invalid file. Usage: ./a1.exe <filePath>\n");
        perror("Error");
        return 1;
    }

    BuildState previousBuildState = buildingWhitespace, buildState = buildingWhitespace,
        lastNonWhitespace = buildingCodeFragment;
    char previousChar = '\0';
    int maxStringSize = MAX_STRING_SIZE, codeCharInt, scopingLevel = 0, tokenCount = 0, 
        maxNumTokens = DEFAULT_LENGTH, tokenCharCount = 0, whitespcaeCharCount = 0,
    maxStringSizeBytes = sizeof(char) * maxStringSize;
    
    char* tempToken = calloc(maxStringSize, maxStringSizeBytes);
    if(tempToken == NULL) {
        printf("Failed to allocate memory.");
        perror("Error");
        return 2;
    }
    char* tempWhitespace = calloc(maxStringSize, maxStringSizeBytes);
    if(tempWhitespace == NULL) {
        printf("Failed to allocate memory.");
        perror("Error");
        return 2;
    }
/*    memset(tempToken, 0, maxStringSizeBytes);
    memset(tempWhitespace, 0, maxStringSizeBytes);*/
    
    /*read all characters from the file to derive useful pieces of information*/
    while((codeCharInt = getc(cPPLFile)) != EOF) {
        /*token too long, double the size of temporary strings*/
        if(tokenCharCount == maxStringSize - 2) {
            maxStringSize *= 2;
            maxStringSizeBytes *= 2;

            char* newStr;
            newStr = calloc(maxStringSize, maxStringSizeBytes);
            if(newStr == NULL) {
                printf("Failed to allocate memory.");
                perror("Error");
                return 2;
            }
            strcpy(newStr, tempToken);
            free(tempToken);
            tempToken = newStr;

            newStr = calloc(maxStringSize, maxStringSizeBytes);
            if(newStr == NULL) {
                printf("Failed to allocate memory.");
                perror("Error");
                return 2;
            }
            strcpy(newStr, tempWhitespace);
            free(tempWhitespace);
            tempWhitespace = newStr;
        }

        /*too many tokens, double the size of the arrays*/
        if(tokenCount == maxNumTokens) {
            maxNumTokens *= 2;

            /*scoping levels*/
            int* newScopingLevels = malloc(sizeof(int) * maxNumTokens);
            if(newScopingLevels == NULL) {
                printf("Failed to allocate memory.");
                perror("Error");
                return 2;
            }
            int i;
            for(i = 0; i < tokenCount; i++) {
                newScopingLevels[i] = (*scopingLevels)[i];
            }
            free(*scopingLevels);
            *scopingLevels = newScopingLevels;

            /*code token types*/
            CodeTokenType* newCodeTokenTypes = malloc(sizeof(CodeTokenType) * maxNumTokens);
            if(newCodeTokenTypes == NULL) {
                printf("Failed to allocate memory.");
                perror("Error");
                return 2;
            }
            for(i = 0; i < tokenCount; i++) {
                newCodeTokenTypes[i] = (*codeTokenTypes)[i];
            }
            free(*codeTokenTypes);
            *codeTokenTypes = newCodeTokenTypes;
        }

        char codeChar = (char)codeCharInt;
        buildState = processCodeChar(codeChar, previousChar, buildState, &scopingLevel);

        if(buildState == buildingWhitespace) {
            /*special cases where the last character belongs to the token*/
            if(previousBuildState == buildingMultilineComment 
            || previousBuildState == buildingStringLiteral) {
                tempToken[tokenCharCount] = codeChar;
                tokenCharCount++;
            }
            /*building whitespace that follows the last code token*/
            else {
                tempWhitespace[whitespcaeCharCount] = codeChar;
                whitespcaeCharCount++;
            }
        }
        /*building the same type of token that is not punctuation*/
        else if((buildState == previousBuildState
            && previousBuildState != singleChar 
            && previousBuildState != buildingArrow)
        || ((buildState == buildingComment 
            || buildState == buildingMultilineComment
            || buildState == buildingArrow)
            && previousBuildState == buildingCodeFragment)) {

            tempToken[tokenCharCount] = codeChar;
            tokenCharCount++;
        }
        /*numbers immediately following letters or underscore belong to an identifier*/
        else if(buildState == buildingNumber && previousBuildState == buildingToken) {
            tempToken[tokenCharCount] = codeChar;
            tokenCharCount++;
            buildState = buildingToken;
        }
        else {
            /*store the code token*/
            if(addNext(codeTokens, tempToken)) {
                return 1;
            }
/*printf("Token: '%s'\n", tempToken);*/

            /*store the code token type*/
            (*codeTokenTypes)[tokenCount] = getCorrespondingTokenType(lastNonWhitespace, tempToken);
/*printf("%d\n", (*codeTokenTypes)[tokenCount]);*/

            /*store scoping level*/
            (*scopingLevels)[tokenCount] = scopingLevel;
            tokenCount++;
/*printf("Scoping Level: %d\n\n", scopingLevel);*/

            /*prepare to build the next code token*/
            tokenCharCount = 0;
            memset(tempToken, 0, strlen(tempToken));
            tempToken[0] = codeChar;
            tokenCharCount++;

            /*store the whitespace following the code token for formatting*/
            if(previousBuildState == buildingWhitespace) {
                if(addNext(followingWhitespace, tempWhitespace)) {
                    return 1;
                }
                /*prepare to build the next whitespace token*/
                whitespcaeCharCount = 0;
                memset(tempWhitespace, 0, strlen(tempWhitespace));
            }
            else {
                /*this prevents the formatting from getting messed up*/
                addNext(followingWhitespace, ""); 
            }
/*printf("Whitespace: '%s'\n", tempWhitespace);*/
        }

        if(buildState != buildingWhitespace) {
            lastNonWhitespace = buildState;
        }
        previousChar = codeChar;
        previousBuildState = buildState;
    }

    /*store last token before EOF*/
    if(tokenCharCount > 0) {
        /*store code token in a collection*/
        if(addNext(codeTokens, tempToken)) {
            return 1;
        }
        (*codeTokenTypes)[tokenCount] = getCorrespondingTokenType(lastNonWhitespace, tempToken);
/*printf("%d\n", (*codeTokenTypes)[tokenCount]);*/
/*printf("Token: '%s'\n", tempToken);*/
        if(previousBuildState == buildingWhitespace) {
            /*store whitespace following the code token in a collection*/
            if(addNext(followingWhitespace, tempWhitespace)) {
                return 1;
            }
/*printf("Whitespace: '%s'\n", tempWhitespace);*/
        }
        else {
            if(addNext(followingWhitespace, "")) {
                return 1;
            }
        }
        (*scopingLevels)[tokenCount] = scopingLevel;
/*printf("Scoping Level: %d\n\n", scopingLevel);*/
    }
/*printTwoDArray(*codeTokens);*/
    free(tempToken);
    free(tempWhitespace);
    fclose(cPPLFile);
    return 0;
}

BuildState processCodeChar(char codeChar, char previousChar, 
    BuildState buildState, int* scopingLevel) {
    switch(buildState) {
        case buildingToken:
        case singleChar:
        case buildingArrow:
        case buildingNumber:
        case buildingCodeFragment:
        case buildingWhitespace:
            return processCodeCharToken(codeChar, previousChar, scopingLevel);
            break;
        case buildingStringLiteral:
            return processCodeCharStringLiteral(codeChar, previousChar);
            break;
        case buildingComment:
            return processCodeCharComment(codeChar);
            break;
        case buildingMultilineComment:
            return processCodeCharMultilineComment(codeChar, previousChar);
            break;
        default:
            printf("%d", buildState);
            printf("An unanticipated error occurred. Build state unhandled.\n");
            exit(1);
    }
}

BuildState processCodeCharToken(char codeChar, char previousChar, int* scopingLevel) {
    switch(codeChar) {
        case ';':
        case '(':
        case ')':
        case ',':
        case '.':
            return singleChar;
            break;
        case '{':
            (*scopingLevel)++;
            return singleChar;
            break;
        case '}':
            (*scopingLevel)--;
            return singleChar;
            break;
        case '"':
            return buildingStringLiteral;
            break;
        case '/':
            if (previousChar == '/') {
                return buildingComment;
            }
            else {
                return buildingCodeFragment;
            }
            break;
        case '*':
            if (previousChar == '/') {
                return buildingMultilineComment;
            }
            else {
                return buildingCodeFragment;
            }
            break;
        case '>':
            if(previousChar == '-') {
                return buildingArrow;
            }
            else {
                return buildingCodeFragment;
            }
            break;
        default:
            if(isspace(codeChar)) {
                return buildingWhitespace;
            }
            else if(isalpha(codeChar) || codeChar == '_') {
                return buildingToken;
            }
            else if(isdigit(codeChar)) {
                return buildingNumber;
            }
            return buildingCodeFragment;
    }
}

BuildState processCodeCharStringLiteral(char codeChar, char previousChar) {
    /*Until " without preceding \ */
    if(codeChar == '"' && previousChar != '\\') {
        return buildingWhitespace;
    }
    else {
        return buildingStringLiteral;
    }
}

BuildState processCodeCharComment(char codeChar) {
    /*Until \n */
    if(codeChar == '\r' || codeChar == '\n') {
        return buildingWhitespace;
    }
    else {
        return buildingComment;
    }
}

BuildState processCodeCharMultilineComment(char codeChar, char previousChar) {
    /*Until *\/ */
    if(codeChar == '/' && previousChar == '*') {
        return buildingWhitespace;
    }
    else {
        return buildingMultilineComment;
    }
}

CodeTokenType getCorrespondingTokenType(BuildState lastNonWhitespace, char* token) {
    switch(lastNonWhitespace) {
        case buildingToken:
            if(isCPPLKeyword(token)) {
                return keyword;
            }
            else {
                return identifier;
            }
            break;
        case buildingComment:
            return comment;
            break;
        case buildingMultilineComment:
            return multilineComment;
            break;
        case buildingStringLiteral:
            return stringLiteral;
            break;
        case buildingNumber:
        case singleChar:
        case buildingArrow:
        case buildingWhitespace:
        case buildingCodeFragment:
        default:
            return special;
            break;
    }
}
