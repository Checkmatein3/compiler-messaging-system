/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "twoDArray.h"

static int doubleSize(TwoDArray** twoDArray) {
    if(twoDArray == NULL || *twoDArray == NULL) {
        return 1;
    }

    TwoDArray* newTwoDArray;
    newTwoDArray = initTwoDArray((*twoDArray)->size * 2);
    int i;
    for(i = 0; i < (*twoDArray)->currentLength; i++) {
        /*strcpy(newTwoDArray->arrayOfStrs[i], (*twoDArray)->arrayOfStrs[i]);*/
        addNext(&newTwoDArray, (*twoDArray)->arrayOfStrs[i]);
    }
    destroyTwoDArray(twoDArray);
    *twoDArray = newTwoDArray;
    return 0;
}

TwoDArray* initTwoDArray(int startSize) {
    if(startSize < 1) {
        return NULL;
    }

    TwoDArray* twoDArray = malloc(sizeof(*twoDArray));
    twoDArray->arrayOfStrs = malloc(sizeof(char*) * startSize);
    twoDArray->size = startSize;
    twoDArray->currentLength = 0;
    return twoDArray;
}
/*
TwoDArray* addAt(TwoDArray* twoDArray, char* str, int pos) {
    if(twoDArray == NULL) {
        return NULL;
    }

    if(pos < 0) {
        return twoDArray;
    }

    if(str == NULL) {
        strcpy(str, "");
    }
    twoDArray->arrayOfStrs[pos] = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(twoDArray->arrayOfStrs[pos], str);
    return twoDArray;
}*/

int addNext(TwoDArray** twoDArray, char* str) {
    if(twoDArray == NULL || *twoDArray == NULL) {
        return 1;
    }

    if(str == NULL) {
        return 1;
    }
    if((*twoDArray)->currentLength == (*twoDArray)->size) {
        if(doubleSize(twoDArray)) {
            return 1;
        }
    }
    int strLength = strlen(str) + 1;
    (*twoDArray)->arrayOfStrs[(*twoDArray)->currentLength] = malloc(sizeof(char) * strLength);
    strcpy((*twoDArray)->arrayOfStrs[(*twoDArray)->currentLength], str);
    (*twoDArray)->currentLength++;
    return 0;
}

void destroyTwoDArray(TwoDArray** twoDArray) {
    if(twoDArray == NULL || *twoDArray == NULL) {
        return;
    }

    int i;
    for(i = 0; i < (*twoDArray)->currentLength; i++) {
/*printf("%s\n", twoDArray->arrayOfStrs[i]);*/
        free((*twoDArray)->arrayOfStrs[i]);
    }
    free((*twoDArray)->arrayOfStrs);
    free(*twoDArray);
    return;
}

int getCurrentLength(TwoDArray* twoDArray) {
    if(twoDArray == NULL) {
        return -1;
    }
    return twoDArray->currentLength;
}

char* getStrAt(TwoDArray* twoDArray, int pos) {
    if(twoDArray == NULL) {
        return NULL;
    }

    if(pos < 0 || pos >= twoDArray->currentLength) {
        return NULL;
    }
    char* strToReturn = malloc(sizeof(char) * strlen(twoDArray->arrayOfStrs[pos]) + 1);
    strcpy(strToReturn,twoDArray->arrayOfStrs[pos]);
    return strToReturn;
}

int getStrLen(TwoDArray* twoDArray, int pos) {
    return strlen(twoDArray->arrayOfStrs[pos]);
}

void printTwoDArray(TwoDArray* twoDArray) {
    int i;
    for(i = 0; i < twoDArray->currentLength; i++) {
        printf("%s\n", twoDArray->arrayOfStrs[i]);
    }
}

TwoDArray* getSubset(int startPos, int endPos, TwoDArray* twoDArray) {
    TwoDArray* subset = initTwoDArray(10);
    int i;
    for(i = startPos; i < endPos; i++) {
        char* str = getStrAt(twoDArray, i);
        if(addNext(&subset, str)) {
            free(str);
            return NULL;
        }
        free(str);
    }
    return subset;
}
