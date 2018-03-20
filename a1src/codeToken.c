/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <string.h>

int isCPPLKeyword(char* token) {
	int i;
	char* keywords[] = {
		"double", "char", "int", "float", "long", "short", "void", 
		"const", "auto", "enum", "break", "case", "continue", 
		"do", "else", "extern", "for", "goto", "if", "register", 
		"signed", "sizeof", "static", "switch", "typedef", "union", 
		"return", "unsigned", "volatile", "while", "default", "struct",
		"class"
	};
	for(i = 0; i < 33; i++) {
		if(strcmp(token, keywords[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
