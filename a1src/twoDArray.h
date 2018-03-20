/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __TWODARRAY_H__
#define __TWODARRAY_H__

typedef struct TwoDArrayStruct {
    char** arrayOfStrs;
    int size;
    int currentLength;
}TwoDArray;

/*
    
    Pre:    
    Post:   
*/
TwoDArray* initTwoDArray(int startSize);

/*
    
    Pre:    
    Post:   
*/
/*TwoDArray* addAt(TwoDArray* twoDArray, char* str, int pos);

*/
/*
    
    Pre:    
    Post:   
*/
int addNext(TwoDArray** twoDArray, char* str);

/*
    
    Pre:    
    Post:   
*/
void destroyTwoDArray(TwoDArray** twoDArray);

/*
    
    Pre:    
    Post:   
*/
int getCurrentLength(TwoDArray* twoDArray);

/*
    
    Pre:    
    Post:   
*/
char* getStrAt(TwoDArray* twoDArray, int pos);

/*
    
    Pre:    
    Post:   
*/
int getStrLen(TwoDArray* twoDArray, int pos);

/*
    
    Pre:    
    Post:   
*/
void printTwoDArray(TwoDArray* twoDArray);

/*
    
    Pre:    
    Post:   
*/
TwoDArray* getSubset(int startPos, int endPos, TwoDArray* twoDArray);

#endif