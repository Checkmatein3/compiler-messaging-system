/*
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 2
    February 17,2017
    This program adds users to streams of a message management and viewing system.
***********************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "addAuthor.h"
#include "stream.h"

/*
    
    Pre:    
    Post:   
*/
int main(int argc, char** argv) {
    int actionStatus = 1;
    if (argc <= 1) {
        printf("Invalid values provided.\n"
                "Usage: ./addauthor [-r] <author-id>\n"
                "List streams: <stream-list>\n");
/*        fprintf(stderr, "[%s][%s][%s]\n", "addAuthor.main", "ERROR", "missing author");*/
        return 1;
    }
    else {
        int i = 1, removeFlag = 0;
        if (strcmp(argv[1], "-r") == 0) {
            removeFlag = 1;
            i++;
        }

        printf("streams: ");
        /*obtain comma separated list of streams*/
        char* streamList = malloc(sizeof(char) * 5000); /*FIXME make me dynamically resize*/
        fgets(streamList, MAX_STREAM_LIST_LEN + 2, stdin);
        if (streamList[strlen(streamList) - 1] == '\n')
        {
            streamList[strlen(streamList) - 1] = '\0';
        }
        else {
            printf("The stream list must not exceed %d characters.\n"
                    "Usage: ./addauthor [-r] <author-id>\n"
                    "List streams: <stream-list>\n", MAX_STREAM_LIST_LEN);
/*            fprintf(stderr, "[%s][%s][%s]\n", "addAuthor.main", "ERROR", "user input exceeded buffer size");*/
/*            free(streamList);*/
            return 1;
        }

        /*remove the author from the specified streams*/
        char* authorName;
        if (removeFlag) {
            if (argc > 2) {
                char** usernameTokens = &(argv[2]);
                authorName = getAuthorName(usernameTokens);
/*printf("author:%s\n", authorName);*/
                actionStatus = removeUser(authorName, streamList);
                if (actionStatus == 1) {
                    printf("Invalid values provided.\n"
                            "Usage: ./addauthor [-r] <author-id>\n"
                            "List streams: <stream-list>\n");
                }
                else if (actionStatus == 2) {
                    printf("A fatal error occurred. Try again.\n");
                }
            }
            else {
                printf("Invalid values provided.\n"
                        "Usage: ./addauthor [-r] <author-id>\n"
                        "List streams: <stream-list>\n");
/*                fprintf(stderr, "[%s][%s][%s]\n", "addAuthor.main", "ERROR", "missing author");*/
/*                free(streamList);*/
                return 1;
            }
        }

        /*add the author to the specified streams*/
        else if (argc > 1) {
            char** usernameTokens = &(argv[1]);
            authorName = getAuthorName(usernameTokens);
/*printf("author:%s\n", authorName);*/
            actionStatus = addUser(authorName, streamList);
            if (actionStatus == 1) {
                printf("Invalid values provided.\n"
                        "Usage: ./addauthor [-r] <author-id>\n"
                        "List streams: <stream-list>\n");
            }
            else if (actionStatus == 2) {
                printf("A fatal error occurred. Try again.\n");
            }
        }
        /*printf("%s", authorName);*/
        free(authorName);
        free(streamList);
    }
    return actionStatus;
}

