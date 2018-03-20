/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stream.h"
#include "twoDArray.h"

int updateStream(struct userPost* st) {
    if (st == NULL || st->streamname == NULL || st->username == NULL
         || st->date == NULL || st->text == NULL
         || (strcmp(st->streamname, "") == 0) || (strcmp(st->username, "") == 0) 
         || (strcmp(st->date, "") == 0) || (strcmp(st->text, "") == 0)) {
        return 1;
    }

    int streamNameLen = strlen(st->streamname);
    char* streamFileName = malloc(sizeof(char) * (streamNameLen + 9 + 6 + 4 + 1));
    if (streamFileName == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.updateStream", "ERROR", "failed to allocate memory");
        return 2;
    }
    sprintf(streamFileName, "messages/%sStream", st->streamname);

    FILE* streamMessagesFile = fopen(streamFileName, "a");
    if (streamMessagesFile == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.updateStream", "ERROR", "unable to open file");
        return 2;
    }

    strcat(streamFileName, "Data");
    FILE* streamDataFile = fopen(streamFileName, "a");
    if (streamDataFile == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.updateStream", "ERROR", "unable to open file");
        return 2;
    }
    free(streamFileName);

    /*update the stream with the new post*/
    fprintf(streamMessagesFile,
            "Sender: %s\n"
            "Date: %s"
            "%s"
            "********************\n", 
            st->username, st->date, st->text);

    /*keep track of the last byte of the message in the data file*/
    long int messageEndPos = ftell(streamMessagesFile) - 1;
    fprintf(streamDataFile, "%ld\n", messageEndPos);

    fclose(streamMessagesFile);
    fclose(streamDataFile);
    return 0;
}

int addUser(char* username, char* list) {
    if (username == NULL || list == NULL) {
        return 1;
    }

    char* listCopy = malloc(sizeof(char) * (strlen(list) + 1));
    if (listCopy == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.addUser", "ERROR", "failed to allocate memory");
        return 2;
    }
    strcpy(listCopy, list);
    char* streamNameToken = strtok(listCopy, ",");
    if (streamNameToken == NULL || strcmp(streamNameToken, "") == 0) {
        free(listCopy);
        return 1;
    } 

    do {
        if (strcmp(streamNameToken, "") == 0) {
            continue;
        }

        char* streamUsersFileName = createStreamUsersFileName(streamNameToken);
        if (streamUsersFileName == NULL) {
            return 2;
        }

        FILE* streamUsersFile = fopen(streamUsersFileName, "a");
        if (streamUsersFile == NULL) {
            fprintf(stderr, "[%s][%s][%s]\n", "stream.addUser", "ERROR", "unable to open file");
            return 2;
        }
        free(streamUsersFileName);

        /*add the user and default number of viewed messages*/
        fprintf(streamUsersFile, "%s 0\n", username);
        
        fclose(streamUsersFile);
    } while ((streamNameToken = strtok(NULL, ",")) != NULL);
    free(listCopy);
    return 0;
}

int removeUser(char* username, char* list) {
    char* listCopy = malloc(sizeof(char) * (strlen(list) + 1));
    if (listCopy == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.removeUser", "ERROR", "failed to allocate memory");
        return 2;
    }
    strcpy(listCopy, list);
    char* streamNameToken = strtok(listCopy, ",");
    if (streamNameToken == NULL || strcmp(streamNameToken, "") == 0) {
        free(listCopy);
        return 1;
    } 
    
    do {
        if (strcmp(streamNameToken, "") == 0) {
            continue;
        }

        char* streamUsersFileName = createStreamUsersFileName(streamNameToken);
        if (streamUsersFileName == NULL) {
            return 2;
        }

        FILE* streamUsersFile = fopen(streamUsersFileName, "r+");
        if (streamUsersFile == NULL) {
            fprintf(stderr, "[%s][%s][%s]\n", "stream.removeUser", "ERROR", "unable to open file");
            return 2;
        }

        /*find the username to remove*/
        char line[MAX_USERNAME_LEN + 2];
        TwoDArray* otherUsers = initTwoDArray(10);
        if (otherUsers == NULL) {
            fprintf(stderr, "[%s][%s][%s]\n", "stream.removeUser", "ERROR", "failed to create twoDArray");
            return 2;
        }
        while (fgets(line, MAX_USERNAME_LEN + 2, streamUsersFile) != NULL) {
            int i, lineLen = strlen(line);
            char currentUsername[MAX_USERNAME_LEN + 2];
            strcpy(currentUsername, line);

            /*username exculding the number of viewed messages*/
            for (i = lineLen - 1; i >= 0; i--) {
                if (currentUsername[i] == ' ') {
                    currentUsername[i] = '\0';
                    break;
                }
            }

            /*keep all other users*/
            if (strcmp(currentUsername, username) != 0) {
                addNext(&otherUsers, line);
            }
        }
        fclose(streamUsersFile);

        /*write back all other users*/
        streamUsersFile = fopen(streamUsersFileName, "w");
        if (streamUsersFile == NULL) {
            fprintf(stderr, "[%s][%s][%s]\n", "stream.removeUser", "ERROR", "unable to open file");
            return 2;
        }
        free(streamUsersFileName);
        
        int i, numFollowingUsers = getCurrentLength(otherUsers);
        for (i = 0; i < numFollowingUsers; i++) {
            char* userBuffer = getStrAt(otherUsers, i);
            if(userBuffer == NULL) {
                fprintf(stderr, "[%s][%s][%s]\n", "stream.removeUser", "ERROR", "failed to retrieve string");
                return 2;
            }
            fputs(userBuffer, streamUsersFile);
            free(userBuffer);
        }
        
        destroyTwoDArray(&otherUsers);
        fclose(streamUsersFile);
    } while ((streamNameToken = strtok(NULL, ",")) != NULL);
    free(listCopy);
    return 0;
}

char* createStreamUsersFileName(char* streamname) {
    char* streamUsersFileName = malloc(sizeof(char) * (strlen(streamname) + 20 + 1));
    if(streamUsersFileName == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.addUser", "ERROR", "failed to allocate memory");
        return NULL;
    }
    sprintf(streamUsersFileName, "messages/%sStreamUsers", streamname);
    return streamUsersFileName;
}

int containsUser(char* streamUsersFileName, char* username) {
    FILE* streamUsersFile = fopen(streamUsersFileName, "r");
    if (streamUsersFile == NULL) {
        fprintf(stderr, "[%s][%s][%s]\n", "stream.removeUser", "ERROR", "unable to open file");
        return 0;
    }

    /*look for the username in the stream users file*/
    char currentUser[MAX_USERNAME_LEN + 2];
    while (fgets(currentUser, MAX_USERNAME_LEN + 2, streamUsersFile) != NULL) {
        int i, currentUserLen = strlen(currentUser);

        /*username exculding the number of viewed messages*/
        for (i = currentUserLen - 1; i >= 0; i--) {
            if (currentUser[i] == ' ') {
                currentUser[i] = '\0';
                break;
            }
        }

        if (strcmp(currentUser, username) == 0) {
            return 1;
        }
    }
    fclose(streamUsersFile);
    return 0;
}

char* getAuthorName(char** authorNameTokens) {
    if (authorNameTokens == NULL) {
        return NULL;
    }

    /*calculate the length of the author name*/
    int i = 0, authorNameLen = 0;
    while (authorNameTokens[i] != NULL) {
        authorNameLen += strlen(authorNameTokens[i]) + 1;
        i++;
    }

    /*construct the author name with space delimited tokens*/
    char* authorName = malloc(sizeof(char) * authorNameLen);
    authorName[0] = '\0';
    i = 0;
    while (authorNameTokens[i] != NULL) {
        strcat(authorName, authorNameTokens[i]);
        if (authorNameTokens[i + 1] != NULL) {
            strcat(authorName, " ");
        }
        i++;
    }
    return authorName;
}
