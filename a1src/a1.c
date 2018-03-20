/*
***********************************************************************************
	Daniel Kolodziejczak

	Assignment 1
	January 27,2017
	This program converts c++light(.cc) source code to c(.c) source code.
***********************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "twoDArray.h"
#include "tokenizer.h"
#include "converter.h"

/*
	This program converts c++light(.cc) source code to c(.c) source code.
	Pre:	argv[1] = c++Light file path
	Post:	the converted source code is in a .c file with the same name as the original
*/
int main(int argc, char** argv) {
    TwoDArray* codeTokens;
    TwoDArray* followingWhitespace;
    CodeTokenType* codeTokenTypes;
    int* scopingLevels;

    if(argc != 2) {
    	printf("File not provided. Usage: ./a1 <file.cc>\n");
    	return 1;
    }

	int fileNameLength = strlen(argv[1]);
    if(argv[1][fileNameLength - 3] != '.' 
    	|| argv[1][fileNameLength - 2] != 'c' 
    	|| argv[1][fileNameLength - 1] != 'c') {
    	printf("Invalid file extension. Usage: ./a1 <file.cc>\n\n");
    	return 1;
    }

	if(tokenizeCPPL(argv[1], &codeTokens, &followingWhitespace, &codeTokenTypes, &scopingLevels) != 0) { /*FIXME SCOPE*/
		destroyTwoDArray(&codeTokens);
		destroyTwoDArray(&followingWhitespace);
		free(codeTokenTypes);
		free(scopingLevels);
		return 1;
	}
	
	argv[1][fileNameLength - 1] = '\0';
	if(convertCPPLToC(&codeTokens, &followingWhitespace, &codeTokenTypes, &scopingLevels, argv[1]) != 0) {
/*printTwoDArray(codeTokens);*/
		destroyTwoDArray(&codeTokens);
		destroyTwoDArray(&followingWhitespace);
		free(codeTokenTypes);
		free(scopingLevels);
		return 1;
	}

	destroyTwoDArray(&codeTokens);
	destroyTwoDArray(&followingWhitespace);
	free(codeTokenTypes);
	free(scopingLevels);
	return 0;
}

/*static destroyTokens(TwoDArray** codeTokens, TwoDArray** followingWhitespace, CodeTokenType** codeTokenTypes) {

}*/