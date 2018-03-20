/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#ifndef __STREAM_H__
#define __STREAM_H__

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
int containsUser(char* streamName, char* username);

/*
    
    Pre:    
    Post:   
*/
char* getAuthorName(char **authorNameTokens);

#endif
