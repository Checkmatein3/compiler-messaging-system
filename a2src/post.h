/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __POST_H__
#define __POST_H__

#include "stream.h"
#include "twoDArray.h"

/*
    
    Pre:    
    Post:   
*/
TwoDArray* readInput();

/*
    
    Pre:    
    Post:   
*/
struct userPost* formatEntry(TwoDArray* inputLines);

/*
    
    Pre:    
    Post:   
*/
char* getTimeDate();

/*
    
    Pre:    
    Post:   
*/
int submitPost(struct userPost* postToSubmit);

#endif
