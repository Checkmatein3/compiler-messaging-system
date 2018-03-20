/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __STREAM_H__
#define __STREAM_H__

#define MAX_USERNAME_LEN 254
#define MAX_STREAM_LIST_LEN 1022
#define MAX_STREAMNAME_LEN 254
#define MAX_DATE_LEN 26
#define MAX_MESSAGE_LINE_LEN 254
#define MAX_TEXT_LEN 1022

struct userPost {
    char *username;
    char *streamname;
    char *date;
    char *text;
};


/*
    
    Pre:    
    Post:   
*/
int updateStream(struct userPost* st);

/*
    
    Pre:    
    Post:   
*/
int addUser(char* username, char* list);

/*
    
    Pre:    
    Post:   
*/
int removeUser(char* username, char* list);

/*
    
    Pre:    
    Post:   
*/
char* createStreamUsersFileName(char* streamname);

/*
    
    Pre:    
    Post:   
*/
int containsUser(char* streamUsersFileName, char* username);

/*
    
    Pre:    
    Post:   
*/
char* getAuthorName(char **authorNameTokens);

#endif
