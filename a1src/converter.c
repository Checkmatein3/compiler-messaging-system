/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "twoDArray.h"
#include "codeToken.h"
#include "converter.h"

int convertCPPLToC(TwoDArray** codeTokens, TwoDArray** followingWhitespace, 
    CodeTokenType** codeTokenTypes, int** scopingLevels, char* outputFilePath) {
    /*ConvertState convertState = globalScope, previousConvertState;*/
    /*int* globalVars = malloc(sizeof(int) * DEFAULT_SIZE);*/
    
    FILE* outputFile;
    outputFile = fopen(outputFilePath, "w");
    if(outputFile == NULL) {
        printf("An unexpected error occurred\n");
        perror("Error");
        return 1;
    }
    int codeTokensLength = getCurrentLength(*codeTokens);/*, i, scopingLevel = 0;*/
/*printTwoDArray(*codeTokens);exit(1);*/
/*printf("C:%d\n", codeTokensLength);
printf("W:%d\n", getCurrentLength(*followingWhitespace));*/

/*test*/
/*dumpToFile(outputFile, 0, codeTokensLength, *codeTokens, *followingWhitespace); 
fclose(outputFile);
return 1; */
    
    /*variables at each scoping level*/
    /*FIXME make me dynamically scaling*/
/*    TwoDArray** variables = malloc(sizeof(TwoDArray*) * 10); 
    for(i = 0; i < 10; i++) {
        TwoDArray[i] = initTwoDArray(10);
    }*/

    TwoDArray* funcIds = initTwoDArray(128);
    TwoDArray* renamedFuncIds = initTwoDArray(128);

    int currentPos = 0;
    /*process class definition*/
    while(1) {
        int classPos = findNextClassDef(currentPos, codeTokensLength, *codeTokens, *codeTokenTypes);
        if(classPos < 0 || classPos >= codeTokensLength) {
            break;
        }

        dumpToFile(outputFile, currentPos, classPos, *codeTokens, *followingWhitespace);
        fputs("struct", outputFile);
        char* wsAfterStruct = getStrAt(*followingWhitespace, classPos);
        fputs(wsAfterStruct, outputFile);
        free(wsAfterStruct);
        
        /*find the start and end of the class declaration*/
        int classStartBracePos = findNext(classPos, codeTokensLength, *codeTokens, "{");
        if(classStartBracePos < 0 || classStartBracePos >= codeTokensLength) {
            return 1;
        }

        int classScopingLevel = (*scopingLevels)[classStartBracePos];
        if(classScopingLevel < 0 || classScopingLevel >= codeTokensLength) {
            return 1;
        }
        int classEndBracePos = findScopingLevel(classStartBracePos, codeTokensLength, *scopingLevels, classScopingLevel - 1);
        if(classEndBracePos < 0 || classEndBracePos >= codeTokensLength) {
            return 1;
        }

/*printf("%d,%d,%d\n", classStartBracePos, classScopingLevel, classEndBracePos);
printf("%s,%s\n", (*codeTokens)->arrayOfStrs[classStartBracePos], 
    (*codeTokens)->arrayOfStrs[classEndBracePos]);
printf("%d,%d\n", (*scopingLevels)[classStartBracePos], (*scopingLevels)[classEndBracePos]);*/

        dumpToFile(outputFile, classPos + 1, classStartBracePos + 2, *codeTokens, *followingWhitespace);
        currentPos = classStartBracePos + 2;

        /*keep track of class names for renaming functions later*/
        char* className = getStrAt(*codeTokens, classPos + 1);

/*        TwoDArray* classVars = getClassVars(classPos + 3, classEndBracePos, codeTokens, codeTokenTypes);*/
        while(currentPos < classEndBracePos) {
            int nextFuncPos = findNextFuncDef(currentPos, classEndBracePos, *codeTokens, *codeTokenTypes, NULL);
            if(nextFuncPos < 0 || nextFuncPos >= classEndBracePos) {
                dumpToFile(outputFile, currentPos, classEndBracePos, *codeTokens, *followingWhitespace);
                break;
            }
            dumpToFile(outputFile, currentPos, nextFuncPos, *codeTokens, *followingWhitespace);

            /*find the start and end of the function definition*/
            int funcStartBracePos = findNext(nextFuncPos, classEndBracePos, *codeTokens, "{");
            if(funcStartBracePos < 0 || funcStartBracePos >= classEndBracePos) {
                return 1;
            }
            int funcScopingLevel = (*scopingLevels)[funcStartBracePos - 1];
            if(funcScopingLevel < 0 || funcScopingLevel >= classEndBracePos) {
                return 1;
            }
            int funcEndBracePos = findScopingLevel(funcStartBracePos, classEndBracePos, *scopingLevels, funcScopingLevel - 1);
            if(funcEndBracePos < 0 || funcEndBracePos >= classEndBracePos) {
                return 1;
            }
/*printf("fb:%s", (*codeTokens)->arrayOfStrs[funcEndBracePos]);*/
            
            /*creating the function pointer*/
            char* funcName = getStrAt(*codeTokens, nextFuncPos);
            TwoDArray* funcCallTokens = getSubset(nextFuncPos + 2, funcStartBracePos - 1, *codeTokens);
            TwoDArray* funcCallWhitespace = getSubset(nextFuncPos + 2, funcStartBracePos - 1, *followingWhitespace);
            TwoDArray* funcParamTypes = getKeywords(nextFuncPos, funcStartBracePos, *codeTokens, *codeTokenTypes);

            /*keep track of fuction names for the current class for renaming functions later*/
            addNext(&funcIds, funcName);
            char* renamedFuncId = createFuncPtrId(className, funcName, funcParamTypes);
            addNext(&renamedFuncIds, renamedFuncId);
            free(renamedFuncId);

            dumpFuncPtrDec(outputFile, className, funcName, funcCallTokens, funcCallWhitespace, funcParamTypes);
            char* wsAfterFunc = getStrAt(*followingWhitespace, funcEndBracePos);
            fputs(wsAfterFunc, outputFile);
            free(wsAfterFunc);
            
            free(funcName);
            destroyTwoDArray(&funcCallTokens);
            destroyTwoDArray(&funcCallWhitespace);
            destroyTwoDArray(&funcParamTypes);

            currentPos = funcEndBracePos + 1;
        }
        free(className);

        /*close off the struct*/
        fputs("};\n", outputFile);

        /*preserve formatting and update current position to after class*/
        char* nextPos = getStrAt(*codeTokens, classEndBracePos + 1);
        if(strcmp(nextPos, ";") == 0) {
            currentPos = classEndBracePos + 2;
        }
        else {
            currentPos = classEndBracePos + 1;
        }
        char* classEndBraceFWS = getStrAt(*followingWhitespace, classEndBracePos);
        fputs(classEndBraceFWS, outputFile);
        free(classEndBraceFWS);
        free(nextPos);
        /*getGlobalVars(i, classPos - 1, globalVars);*/
    }
/*    fclose(outputFile);
return 1;*/
/*            TwoDArray* funcParams = getVars(nextFuncPos, funcStartBracePos, codeTokens, codeTokenTypes);
            destroyTwoDArray(&funcParams);
*/
    if(dumpMethodDefs(outputFile, 0, currentPos, *codeTokens, *followingWhitespace, *codeTokenTypes, *scopingLevels)) {
        return 1;
    }

    int mainPos = findMain(*codeTokens);
    if(mainPos >= 0) {
        /*find the start and end of main*/
        int mainStartBracePos = findNext(mainPos, codeTokensLength, *codeTokens, "{");
        if(mainStartBracePos < 0 || mainStartBracePos >= codeTokensLength) {
            return 1;
        }
        int mainScopingLevel = (*scopingLevels)[mainStartBracePos];
        if(mainScopingLevel < 0 || mainScopingLevel >= codeTokensLength) {
            return 1;
        }
        int mainEndBracePos = findScopingLevel(mainStartBracePos, codeTokensLength, *scopingLevels, mainScopingLevel - 1);
        if(mainEndBracePos < 0 || mainEndBracePos >= codeTokensLength) {
            return 1;
        }
        dumpToFile(outputFile, currentPos, mainStartBracePos + 1, *codeTokens, *followingWhitespace);

        for(currentPos = mainStartBracePos + 1; currentPos <= mainEndBracePos; currentPos++) {
            char* className;
            char* currentToken = getStrAt(*codeTokens, currentPos);
            /*add constructor for each object declaration*/
            if(isObjDec(currentPos, *codeTokens, *codeTokenTypes)) {
                if ((*codeTokenTypes)[currentPos - 1] == identifier) {
                    className = getStrAt(*codeTokens, currentPos - 1);
                }
                else {
                    className = getStrAt(*codeTokens, currentPos - 2);
                }

                /*TwoDArray* sameLineObjDecIds = initTwoDArray(10);*/
                int nextSemicolonPos = findNext(currentPos, mainEndBracePos, *codeTokens, ";");
/*                int nextObjDecIdPos;
                while((nextObjDecIdPos = findNextObjDec(currentPos, mainEndBracePos, *codeTokens, *codeTokenTypes)) > 0
                    && nextObjDecIdPos < nextSemicolonPos) {
                    dumpToFile(outputFile, currentPos, nextObjDecIdPos + 1, *codeTokens, *followingWhitespace);
                    char* objDecId = getStrAt(*codeTokens, nextObjDecIdPos);
                    currentPos = nextObjDecIdPos + 1;
                    addNext(&sameLineObjDecIds, objDecId);
                    free(objDecId);
                }
                dumpToFile(outputFile, currentPos, nextSemicolonPos + 1, *codeTokens, *followingWhitespace);
                int i, numSameLineObjDecIds = getCurrentLength(sameLineObjDecIds);
                for(i = 0; i < numSameLineObjDecIds; i++) {
                    char* objDecId = getStrAt(sameLineObjDecIds, i);
                    dumpStructConstructorCall(outputFile, objDecId, className);
                    free(objDecId);
                }*/

                /*NEW*/
                char* objDecId = getStrAt(*codeTokens, currentPos);
                dumpToFile(outputFile, currentPos, nextSemicolonPos + 1, *codeTokens, *followingWhitespace);
                currentPos = nextSemicolonPos;
                dumpStructConstructorCall(outputFile, objDecId, className);
                free(objDecId);

                free(className);
                /*destroyTwoDArray(&sameLineObjDecIds);*/
            }
            else if(strcmp(currentToken, "class") == 0) {
                fputs("struct", outputFile);
                char* followingWS = getStrAt(*followingWhitespace, currentPos);
                fputs(followingWS, outputFile);
                free(followingWS);
            }
            else if(isFuncPtrCall(currentPos, *codeTokens, *codeTokenTypes)) {
                int i, numFuncIds = getCurrentLength(funcIds);
                char* funcPtrCallId = getStrAt(*codeTokens, currentPos);
                for(i = 0; i < numFuncIds; i++) {
                    char* funcId = getStrAt(funcIds, i);
                    if(strcmp(funcId, funcPtrCallId) == 0) {
                        char* newFuncName = getStrAt(renamedFuncIds, i);
                        (strcmp((*codeTokens)->arrayOfStrs[++currentPos + 1], ")") != 0) ? fprintf(outputFile, "%s%s%s, ", newFuncName, (*codeTokens)->arrayOfStrs[currentPos], (*codeTokens)->arrayOfStrs[currentPos - 3]) : fprintf(outputFile, "%s%s%s", newFuncName, (*codeTokens)->arrayOfStrs[currentPos], (*codeTokens)->arrayOfStrs[currentPos - 3]);
                        char* followingWS = getStrAt(*followingWhitespace, currentPos);
                        fputs(followingWS, outputFile);
                        free(followingWS);
                        free(newFuncName);
                    }
                    free(funcId);
                }
                free(funcPtrCallId);
            }
            else {
                dumpToFile(outputFile, currentPos, currentPos + 1, *codeTokens, *followingWhitespace);
            }
            free(currentToken);
        }
    }
    destroyTwoDArray(&funcIds);
    destroyTwoDArray(&renamedFuncIds);
    dumpToFile(outputFile, currentPos, codeTokensLength, *codeTokens, *followingWhitespace);
    return 0;
}

void dumpFuncPtrDec(FILE* outputFile, char* className, char* funcName, 
    TwoDArray* funcCallTokens, TwoDArray* funcCallWhitespace, 
    TwoDArray* funcParamTypes) {
    int funcCallTokensLength = getCurrentLength(funcCallTokens);

    fputs("(*", outputFile);
    char* newFuncPtrId = createFuncPtrId(className, funcName, funcParamTypes);
    fputs(newFuncPtrId, outputFile);
    free(newFuncPtrId);
    fputs(")(", outputFile);
    fprintf(outputFile, "struct %s* my%s", className, className);
    if(getCurrentLength(funcCallTokens) > 0) {
        fputs(", ", outputFile);
    }
    dumpToFile(outputFile, 0, funcCallTokensLength, funcCallTokens, funcCallWhitespace);
    fputs(");", outputFile);
    return;
}

char* createFuncPtrId(char* className, char* funcName, TwoDArray* funcParamTypes) {
    int i, paramTypesLength = getCurrentLength(funcParamTypes), funcNameLen = strlen(funcName);
    char* funcPtrId = malloc(sizeof(char) * (paramTypesLength + 2 + funcNameLen));
    sprintf(funcPtrId, "%c%s", className[0], funcName);
    int currentLen = funcNameLen + 1;
    for(i = 0; i < paramTypesLength; i++) {
        char* typeStr = getStrAt(funcParamTypes, i);
        funcPtrId[currentLen + i] = typeStr[0];
        free(typeStr);
    }
    funcPtrId[currentLen + i] = '\0';
    return funcPtrId;
}

int dumpToFile(FILE* outputFile, int startToken, int endToken, 
    TwoDArray* codeTokens, TwoDArray* followingWhitespace) {
    int i;
    for(i = startToken; i < endToken; i++) {
        char* tempStr = getStrAt(codeTokens, i);
        if(tempStr != NULL) {
            fputs(tempStr, outputFile);
            free(tempStr);
        }
        tempStr = getStrAt(followingWhitespace, i);
        if(tempStr != NULL) {
            fputs(tempStr, outputFile);
            free(tempStr);
        }
    }
    return 0;
}

/*
void dumpBlockToFile(FILE* outputFile, int startToken, int endToken,
    TwoDArray* codeTokens, TwoDArray* followingWhitespace, 
    CodeTokenType* codeTokenTypes, int* scopingLevels, 
    TwoDArray* classNames, TwoDArray* classVars[], TwoDArray* classMethods[]) {
    int i;
    TwoDArray* globalVars = get
    for(i = startToken; i < endToken; i++) {
        int varIdOffset;
        if(isFuncDef(i, codeTokens, codeTokenTypes)) {
            if(isMain(i, codeTokens)) {
                int mainStartBrace = findNext(i, codeTokensLength, codeTokens, "{");
                dumpToFile(outputFile, i, mainStartBrace + 1, codeTokens, followingWhitespace);
                i = mainStartBrace;
                int j, numClassNames = getCurrentLength(classNames);
                for(j = 0; j < numClassNames; j++) {
                    dumpStructConstructorCall(outputFile, className);
                }
            }
            else {
            }
        }*/
    /*    else if(isVarDec(i, codeTokens, codeTokenTypes, &varIdOffset)) {

        }*/
/*        else {
            dumpToFile(outputFile, i, i + 1, codeTokens, followingWhitespace);
        }
    }

    
    return;
}
*/
TwoDArray* getVars(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    TwoDArray* vars = initTwoDArray(10);
    int varPos = startToken - 1;
    while((varPos = findNextTokenType(varPos + 1, endToken, codeTokens, codeTokenTypes, identifier)) >= 0) {
        char* varToken = getStrAt(codeTokens, varPos);
        if(addNext(&vars, varToken)) {
            return NULL;
        }
    }
    return vars;
}

TwoDArray* getVarDecs(int startToken, int endToken,
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    TwoDArray* varDecs = initTwoDArray(10);
    int varDecPos = startToken - 1;
    while((varDecPos = findNextVarDec(varDecPos + 1, endToken, codeTokens, codeTokenTypes)) >= 0) {
        char* varDecToken = getStrAt(codeTokens, varDecPos);
        if(addNext(&varDecs, varDecToken)) {
            return NULL;
        }
    }
    return varDecs;
}

/*
void getGlobalVars(int startToken, int endToken, int scopingLevel,
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes, int* scopingLevels) {
    for(i = startToken; i <= endToken; i++) {
        if(scopingLevels[i] == 0) {
            char* followingToken = getStrAt(codeTokens, i + 1);
            if(codeTokenTypes[i] == keyword && followingToken == identifier
            &&(strcmp(precedingToken, )))
        }
    }
}
*/
int findScopingLevel(int startToken, int endToken, 
    int* scopingLevels, int scopingLevel) {
    int i;
    for(i = startToken; i < endToken; i++) {
        if(scopingLevels[i] == scopingLevel) {
            i += 1; /*quick fix for scoping levels off by one*/
            return i;
        }
    }
    return -1;
}

TwoDArray* getKeywords(int startToken, int endToken,
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    TwoDArray* funcParamTypes = initTwoDArray(10);
    int keywordPos = startToken - 1;
    while((keywordPos = findNextTokenType(keywordPos + 1, endToken, codeTokens, codeTokenTypes, keyword)) >= 0) {
        char* keywordToken = getStrAt(codeTokens, keywordPos);
        if(addNext(&funcParamTypes, keywordToken)) {
            free(keywordToken);
            return NULL;
        }
        free(keywordToken);
    }
    return funcParamTypes;
}

int dumpMethodDefs(FILE* outputFile, int startPos, int endPos, 
    TwoDArray* codeTokens, TwoDArray* followingWhitespace, 
    CodeTokenType* codeTokenTypes, int* scopingLevels) {
    int currentPos = startPos, codeTokensLength = getCurrentLength(codeTokens), classEndBracePos;
    while(1) {
        int classPos = findNextClassDef(currentPos, codeTokensLength, codeTokens, codeTokenTypes);
        if(classPos < 0 || classPos >= codeTokensLength) {
            break;
        }
        char* className = getStrAt(codeTokens, classPos + 1);

        /*find the start and end of the class declaration*/
        int classStartBracePos = findNext(classPos, codeTokensLength, codeTokens, "{");
        if(classStartBracePos < 0 || classStartBracePos >= codeTokensLength) {
            return 1;
        }

        int classScopingLevel = scopingLevels[classStartBracePos];
        if(classScopingLevel < 0 || classScopingLevel >= codeTokensLength) {
            return 1;
        }
        classEndBracePos = findScopingLevel(classStartBracePos, codeTokensLength, scopingLevels, classScopingLevel - 1);
        if(classEndBracePos < 0 || classEndBracePos >= codeTokensLength) {
            return 1;
        }

        TwoDArray* classMethodNames = initTwoDArray(10);
        TwoDArray* funcPtrs = initTwoDArray(10);
        currentPos = classStartBracePos + 2;
        int firstOccurrence = 1;
        while(currentPos < classEndBracePos) {
            int firstTypePos;
            int nextFuncPos = findNextFuncDef(currentPos, classEndBracePos, codeTokens, codeTokenTypes, &firstTypePos);
            if(nextFuncPos < 0 || nextFuncPos >= classEndBracePos) {
                break;
            }

            /*find the start and end of the function definition*/
            int funcStartBracePos = findNext(nextFuncPos, classEndBracePos, codeTokens, "{");
            if(funcStartBracePos < 0 || funcStartBracePos >= classEndBracePos) {
                return 1;
            }
            int funcScopingLevel = scopingLevels[funcStartBracePos - 1];
            if(funcScopingLevel < 0 || funcScopingLevel >= classEndBracePos) {
                return 1;
            }
            int funcEndBracePos = findScopingLevel(funcStartBracePos, classEndBracePos, scopingLevels, funcScopingLevel - 1);
            if(funcEndBracePos < 0 || funcEndBracePos >= classEndBracePos) {
                return 1;
            }

            /*preserve formatting*/
            if(firstOccurrence) {
                char* precedingWS = getStrAt(followingWhitespace, firstTypePos - 1);
                fputs(precedingWS, outputFile);
                free(precedingWS);
                firstOccurrence = 0;
            }

            dumpToFile(outputFile, firstTypePos, nextFuncPos, codeTokens, followingWhitespace);

            /*original function name replaced with the new function identifier*/
            char* funcName = getStrAt(codeTokens, nextFuncPos);
            addNext(&classMethodNames, funcName);

            TwoDArray* funcParamTypes = getKeywords(nextFuncPos, funcStartBracePos, codeTokens, codeTokenTypes);
            
            char* newFuncPtrId = createFuncPtrId(className, funcName, funcParamTypes);
            fputs(newFuncPtrId, outputFile);
            addNext(&funcPtrs, newFuncPtrId);
            free(newFuncPtrId);

            /*add struct to function paramaters and dump rest of function*/
            dumpToFile(outputFile, nextFuncPos + 1, nextFuncPos + 2, codeTokens, followingWhitespace);
            fprintf(outputFile, "struct %s* my%s", className, className);
            if (strcmp(codeTokens->arrayOfStrs[nextFuncPos + 2], ")") != 0) {
                fputs(", ", outputFile);
            }
            dumpToFile(outputFile, nextFuncPos + 2, funcEndBracePos + 1, codeTokens, followingWhitespace);

            free(funcName);
            destroyTwoDArray(&funcParamTypes);

            currentPos = funcEndBracePos + 1;
        }

        /*link the functions with the class*/
        dumpStructConstructorDef(outputFile, className, funcPtrs, scopingLevels[currentPos]);
        free(className);
        destroyTwoDArray(&classMethodNames);
        destroyTwoDArray(&funcPtrs);

        /*preserve formatting and update currentPos*/
        char* nextPos = getStrAt(codeTokens, classEndBracePos + 1);
        if(strcmp(nextPos, ";") == 0) {
            currentPos = classEndBracePos + 2;
        }
        else {
            currentPos = classEndBracePos + 1;
        }
        free(nextPos);
        char* classEndBraceFWS = getStrAt(followingWhitespace, currentPos);
        fputs(classEndBraceFWS, outputFile);
        free(classEndBraceFWS);
    }

    return 0;
}

void dumpStructConstructorCall(FILE* outputFile, const char* objDecId, const char* className) {
    fprintf(outputFile, "constructor%s(%s);\n", className, objDecId);
    return;
}

void dumpStructConstructorDef(FILE* outputFile, const char* className, 
    TwoDArray* funcPtrs, int scopingLevel) {
    int i, numFuncPtrs = getCurrentLength(funcPtrs);

    fprintf(outputFile, "\nvoid constructor%s(struct %s* my%s) {\n", className, className, className);
    for(i = 0; i < numFuncPtrs; i++) {
        char* funcPtr = getStrAt(funcPtrs, i);
        int j;
        for(j = 0; j < scopingLevel + 1; j++) {
            fputc('\t', outputFile);
        }
        fprintf(outputFile, "my%s->%s = &%s;\n", className, funcPtr, funcPtr);
        free(funcPtr);
    }

    for(i = 0; i < scopingLevel; i++) {
        fputc('\t', outputFile);
    }
    fputs("}\n", outputFile);
    return;
}

int findNextFuncDef(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes, int* firstTypePos) {
    int i;
    for(i = startToken; i < endToken; i++) {
        if(isFuncDef(i, codeTokens, codeTokenTypes, firstTypePos)) {
            return i;
        }
    }
    return -1;
}

int isFuncDef(int tokenPos, TwoDArray* codeTokens, 
    CodeTokenType* codeTokenTypes, int* firstTypePos) {
    char* ctPos1 = getStrAt(codeTokens, tokenPos - 4);
    char* ctPos2 = getStrAt(codeTokens, tokenPos - 3);
    char* ctPos3 = getStrAt(codeTokens, tokenPos - 2);
    char* ctPos4 = getStrAt(codeTokens, tokenPos - 1);
    char* ctPos5 = getStrAt(codeTokens, tokenPos);
    char* ctPos6 = getStrAt(codeTokens, tokenPos + 1);
    CodeTokenType ctTypePos1 = codeTokenTypes[tokenPos - 4];
    CodeTokenType ctTypePos2 = codeTokenTypes[tokenPos - 3];
    CodeTokenType ctTypePos3 = codeTokenTypes[tokenPos - 2];
    CodeTokenType ctTypePos4 = codeTokenTypes[tokenPos - 1];
    CodeTokenType ctTypePos5 = codeTokenTypes[tokenPos];
    int retVal = 0;

    if(((ctTypePos2 == keyword
        && ctTypePos3 == keyword
        && ctTypePos4 == special
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0))
    || ((ctTypePos2 == keyword
        && ctTypePos3 == keyword
        && ctTypePos4 == keyword
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0))
    || (((strcmp(ctPos2, "struct") == 0 
        || strcmp(ctPos2, "class") == 0)
        && ctTypePos3 == identifier
        && ctTypePos4 == special
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0))) {
        if(firstTypePos != NULL) {
            *firstTypePos = tokenPos - 3;
        }
        retVal = 1;
    }
    else if(((ctTypePos3 == keyword
        && ctTypePos4 == special
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0)) 
    || ((ctTypePos3 == keyword
        && ctTypePos4 == keyword
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0))
    || (((strcmp(ctPos3, "struct") == 0 
        || strcmp(ctPos3, "class") == 0)
        && ctTypePos4 == identifier
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0))
    || (ctTypePos3 == identifier
        && ctTypePos4 == special
        && ctTypePos5 == identifier)) {
        if(firstTypePos != NULL) {
            *firstTypePos = tokenPos - 2;
        }
        retVal = 1;
    }
    else if((ctTypePos1 == keyword
        && ctTypePos2 == keyword
        && ctTypePos3 == keyword
        && ctTypePos4 == special
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0)) {
        if(firstTypePos != NULL) {
            *firstTypePos = tokenPos - 4;
        }
        retVal = 1;
    }
    else if((ctTypePos4 == keyword || ctTypePos4 == identifier)
        && ctTypePos5 == identifier
        && strcmp(ctPos6, "(") == 0) {
        if(firstTypePos != NULL) {
            *firstTypePos = tokenPos - 1;
        }
        retVal = 1;
    }
    free(ctPos1);
    free(ctPos2);
    free(ctPos3);
    free(ctPos4);
    free(ctPos5);
    free(ctPos6);
    return retVal;
}

int findMain(TwoDArray* codeTokens) {
    int i, numCodeTokens = getCurrentLength(codeTokens);
    for(i = 0; i < numCodeTokens; i++) {
        if(isMain(i, codeTokens)) {
            return i;
        }
    }
    return -1;
}

int isMain(int tokenPos, TwoDArray* codeTokens) {
    int retVal = 0;
    char* ctPos1 = getStrAt(codeTokens, tokenPos);
    char* ctPos2 = getStrAt(codeTokens, tokenPos + 1);
    if(strcmp(ctPos1, "main") == 0
        && strcmp(ctPos2, "(") == 0) {
        retVal = 1;
    }
    free(ctPos1);
    free(ctPos2);
    return retVal;
}

int findNextClassDef(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    int i;
    for(i = startToken; i < endToken; i++) {
        char* ctPos1 = getStrAt(codeTokens, i);
        CodeTokenType ctTypePos2 = codeTokenTypes[i + 1];
        char* ctPos3 = getStrAt(codeTokens, i + 2);
        if(strcmp(ctPos1, "class") == 0
            && ctTypePos2 == identifier
            && strcmp(ctPos3, "{") == 0) {
            free(ctPos1);
            free(ctPos3);
            return i;
        }
        free(ctPos1);
        free(ctPos3);
    }
    return -1;
}

int findNextTokenType(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes, 
    CodeTokenType codeTokenType) {
    int i;
    for(i = startToken; i < endToken; i++) {
        CodeTokenType ctTypePos1 = codeTokenTypes[i];
        if(ctTypePos1 == codeTokenType) {
            return i;
        }
    }
    return -1;
}

int findNextObjDec(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    int i;
    for(i = startToken; i < endToken; i++) {
        if(isObjDec(i, codeTokens, codeTokenTypes)) {
            return i;
        }
    }
    return -1;
}

int isObjDec(int objIdPos, TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    int classTokenPos = findPrev(0, objIdPos, codeTokens, "class");
    if(classTokenPos < 0) {
        return 0;
    }
    int prevSemicolonTokenPos = findPrev(0, objIdPos, codeTokens, ";");
    int prevBraceTokenPos = findPrev(0, objIdPos, codeTokens, "{");
    CodeTokenType ctTypeAfterClass = codeTokenTypes[classTokenPos + 1];
    CodeTokenType objIdType = codeTokenTypes[objIdPos];
    if(classTokenPos > prevSemicolonTokenPos && classTokenPos > prevBraceTokenPos
        && ctTypeAfterClass == identifier && objIdType == identifier
        && classTokenPos + 1 != objIdPos) {
        return 1;
    }
    return 0;
}

int isFuncPtrCall(int tokenPos, TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
    char* ctPos1 = getStrAt(codeTokens, tokenPos - 1);
    CodeTokenType ctTypePos2 = codeTokenTypes[tokenPos];
    char* ctPos3 = getStrAt(codeTokens, tokenPos + 1);
    int retVal = 0;
    if(((strcmp(ctPos1, ".") == 0) 
        || (strcmp(ctPos1, "->") == 0))
        && ctTypePos2 == identifier
        && strcmp(ctPos3, "(") == 0) {
        retVal = 1;
    }
    free(ctPos1);
    free(ctPos3);
    return retVal;
}

int findNextVarDec(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
/*    int i;
    for(i = startToken; i < endToken; i++) {
        char* ctPos2 = getStrAt(codeTokens, i + 1);
        CodeTokenType ctTypePos1 = codeTokenTypes[i];
        if(strcmp(ctPos1, "\0") == 0) {
            return i;
        }
        else if(strcmp(ctPos1, "\0") == 0) {
            return i;
        }
        free(ctPos1);
        free(ctPos2);
        free(ctPos3);
    }*/
    return 0;
}

int findNextVarDef(int startToken, int endToken, 
    TwoDArray* codeTokens, CodeTokenType* codeTokenTypes) {
/*    int i;
    for(i = startToken; i < endToken; i++) {
        char* ctPos1 = getStrAt(codeTokens, i);
        char* ctPos2 = getStrAt(codeTokens, i + 1);
        char* ctPos3 = getStrAt(codeTokens, i + 2);
        CodeTokenType ctTypePos1 = codeTokenTypes[i];
        CodeTokenType ctTypePos2 = codeTokenTypes[i + 1];
        CodeTokenType ctTypePos3 = codeTokenTypes[i + 2];
        if(strcmp(ctPos1, "\0") == 0) {
            return i;
        }
        else if(strcmp(ctPos1, "\0") == 0) {
            return i;
        }
        free(ctPos1);
        free(ctPos2);
        free(ctPos3);
    }*/
    return 0;
}



int findNext(int startToken, int endToken, 
    TwoDArray* codeTokens, const char* tokenToFind) {
    int i;
    for(i = startToken; i < endToken; i++) {
        char* ctPos1 = getStrAt(codeTokens, i);
        if(strcmp(ctPos1, tokenToFind) == 0) {
            free(ctPos1);
            return i;
        }
        free(ctPos1);
    }
    return -1;
}

int findPrev(int startToken, int endToken, 
    TwoDArray* codeTokens, const char* tokenToFind) {
    int i;
    for(i = endToken - 1; i > startToken; i--) {
        char* ctPos1 = getStrAt(codeTokens, i);
        if(strcmp(ctPos1, tokenToFind) == 0) {
            free(ctPos1);
            return i;
        }
        free(ctPos1);
    }
    return -1;
}
