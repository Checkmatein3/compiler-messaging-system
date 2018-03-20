/*
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 2
    February 17,2017
    This program allows users to post messages to streams of a message management
    and viewing system.
***********************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "post.h"
#include "stream.h"
#include "twoDArray.h"
#include "dbutil.h"

class postEntry {
    char* readInput(char** streamname) {
        /*obtain the streamname*/
        printf("stream: ");
        *streamname = malloc(sizeof(char) * MAX_STREAMNAME_LEN + 2);
        fgets(*streamname, MAX_STREAMNAME_LEN + 2, stdin);

        int streamNameLen = strlen(*streamname);
        if (*streamname == NULL || streamNameLen < 1) {
            printf("The streamname must be at least one character long.\n");
            free(*streamname);
            return NULL;
        }
        else if ((*streamname)[streamNameLen - 1] != '\n') {
            printf("The streamname must not exceed %d characters.\n", MAX_STREAMNAME_LEN);
            free(*streamname);
            return NULL;
        }
        else {
            (*streamname)[streamNameLen - 1] = '\0';
        }

        /*obtain the message*/
        printf("enter text: ");
        int i = 0, textLen = 10;
        char *textLine = malloc(sizeof(char) * MAX_MESSAGE_LINE_LEN), newChar;

        while((newChar = getchar()) != EOF) { 
            textLine[i++] = newChar; 
            if (i == textLen) { 
                textLine = realloc(textLine, textLen*=2);
                if (textLine == NULL) {
                    fprintf(stderr, "[%s][%s][%s]\n", "post.readInput", "ERROR", "realloc failed.\n");
                    return NULL;
                }
            }
        }
        textLine[i++]='\0';
        printf("\n");
        return textLine;
    }

    struct userPost* formatEntry(char*username, char* streamname, char* date, char* text) {
        struct userPost* newUserPost = malloc(sizeof(struct userPost));
        if (newUserPost == NULL) {
            return NULL;
        }

        newUserPost->username = username;
        newUserPost->streamname = streamname;
        newUserPost->text = text;
        newUserPost->date = date;
        return newUserPost;
    }

    char* getTimeDate() {
        time_t calendarTime = time(NULL);
        struct tm* localTimeDate = localtime(&calendarTime);
        char* date = asctime(localTimeDate);
        return date;
    }

    int submitPost(struct userPost* postToSubmit) {
        /*check that the user has permission to post to the stream*/
        if (!containsUser(postToSubmit->streamname, postToSubmit->username)) {
            printf("The user '%s' does not have permission to post to the '%s' stream.\n", postToSubmit->username, postToSubmit->streamname);
            return 1;
        }
        else {
            int submissionStatus = updateStream(postToSubmit);
            if (submissionStatus != 0) {
                printf("The post could not be made at this time. Please try again.\n");
                return 1;
            }
        }
/*        printf("%s\n%s\n%s\n%s\n", postToSubmit->username, postToSubmit->streamname, postToSubmit->date, postToSubmit->text);*/
        return 0;
    }
}

/*

    Pre:
    Post:
*/
int main(int argc, char** argv) {
    /*obtain the post data*/
    /*obtain username*/
    char** usernameTokens = &(argv[1]);
    char* username = getAuthorName(usernameTokens);

    if (username == NULL) {
        printf("No author provided.\n");
        return 1;
    }
    else if(username[0] == '\0') {
        printf("The username must be at least one character long.\n");
        free(username);
        return 1;
    }

    class postEntry* newPostEntry = malloc(sizeof(struct postEntry));
    char* streamname;
    char* text = newPostEntry->readInput(&streamname);
    if (text == NULL || streamname == NULL) {
        free(username);
        free(newPostEntry);
        return 1;
    }

    char* date = newPostEntry->getTimeDate();
    if (date == NULL || date[0] == '\0') {
        printf("The post could not be made at this time. Please try again.\n");
        fprintf(stderr, "[%s][%s][%s]\n", "post.main", "ERROR", "unable to get current time and date\n");

        free(text);
        free(streamname);
        free(username);
        free(newPostEntry);
        return 2;
    }

    /*format the post and submit it*/
    struct userPost* newUserPost = newPostEntry->formatEntry(username, streamname, date, text);
    if (newUserPost == NULL) {
        printf("The post could not be made at this time. Please try again.\n");
        fprintf(stderr, "[%s][%s][%s]\n", "post.main", "ERROR", "failed to allocate memory for post\n");

        free(text);
        free(streamname);
        free(username);
        free(newPostEntry);
        return 2;
    }
    int postStatus = newPostEntry->submitPost(newUserPost);

    free(text);
    free(streamname);
    free(username);
    free(newUserPost);
    free(newPostEntry);
    return postStatus;
}
