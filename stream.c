/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "stream.h"
#include "dbutil.h"

/*
    Commands to create tables
    
    CREATE TABLE Streams (
        stream_name VARCHAR(100),
        PRIMARY KEY (stream_name)
    );

    CREATE TABLE Users (
        username VARCHAR(100),
        PRIMARY KEY (username)
    );

    CREATE TABLE IF NOT EXISTS StreamUsers (
        stream_name VARCHAR(100) NOT NULL,
        username VARCHAR(100) NOT NULL,
        num_posts_read int NOT NULL,
        PRIMARY KEY (stream_name, username)
    );

    CREATE TABLE IF NOT EXISTS Stream (
        post_id INT NOT NULL AUTO_INCREMENT,
        stream_name VARCHAR(100) NOT NULL,
        author VARCHAR(100) NOT NULL,
        message VARCHAR(5000) NOT NULL,
        date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (stream_name, author) REFERENCES StreamUsers(stream_name, username),
        PRIMARY KEY (post_id)
    );
*/

int updateStream(struct userPost* st) {
    if (st == NULL || st->streamname == NULL || st->username == NULL
         || st->date == NULL || st->text == NULL
         || (strcmp(st->streamname, "") == 0) || (strcmp(st->username, "") == 0) 
         || (strcmp(st->date, "") == 0) || (strcmp(st->text, "") == 0)) {
        return 1;
    }

    char formatStr[MAX_QUERY];

    strcpy(
        formatStr,
        "CREATE TABLE IF NOT EXISTS Stream ("
        "    post_id INT NOT NULL AUTO_INCREMENT,"
        "    stream_name VARCHAR(%d) NOT NULL,"
        "    author VARCHAR(%d) NOT NULL,"
        "    message VARCHAR(%d) NOT NULL,"
        "    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
        "    FOREIGN KEY (stream_name, author) REFERENCES StreamUsers(stream_name, username),"
        "    PRIMARY KEY (post_id)"
        ");"
    );
    char createStreamTableQuery[MAX_QUERY];
    sprintf(createStreamTableQuery, formatStr, MAX_STREAMNAME_LEN, MAX_USERNAME_LEN, MAX_TEXT_LEN);
    
    strcpy(formatStr, "INSERT INTO Stream VALUES (DEFAULT,'%s','%s','%s',DEFAULT);");
    char postToStreamQuery[MAX_QUERY];
    sprintf(postToStreamQuery, formatStr, st->streamname, st->username, st->text);
    
    MYSQL mysql;
    connectToDB(&mysql);
    
    mysql_query(&mysql, createStreamTableQuery);
    mysql_query(&mysql, postToStreamQuery);

    mysql_close(&mysql);
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

        char formatStr[MAX_QUERY];

        strcpy(
            formatStr, 
            "CREATE TABLE IF NOT EXISTS StreamUsers ("
            "    stream_name VARCHAR(%d) NOT NULL,"
            "    username VARCHAR(%d) NOT NULL,"
            "    last_post_read int NOT NULL,"
            "    PRIMARY KEY (stream_name, username)"
            ");"
        );

        char createStreamUsersTableQuery[MAX_QUERY];
        sprintf(createStreamUsersTableQuery, formatStr, MAX_STREAMNAME_LEN, MAX_USERNAME_LEN);

        strcpy(formatStr, "INSERT INTO StreamUsers VALUES ('%s','%s',0);");
        
        char addUserToStreamQuery[MAX_QUERY];
        sprintf(addUserToStreamQuery, formatStr, streamNameToken, username);
        
        MYSQL mysql;
        connectToDB(&mysql);

        mysql_query(&mysql, createStreamUsersTableQuery);
        mysql_query(&mysql, addUserToStreamQuery);
        
        mysql_close(&mysql);

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

        /* remove the user from the stream */
        char *formatStr = "DELETE FROM StreamUsers WHERE stream_name = '%s' AND username = '%s';";
        char streamUserInsertion[MAX_QUERY];
        sprintf(streamUserInsertion, formatStr, streamNameToken, username);
        
        MYSQL mysql;
        connectToDB(&mysql);

        mysql_query(&mysql, streamUserInsertion);

        mysql_close(&mysql);

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



int containsUser(char* streamName, char* username) {
    char *formatStr = "SELECT * FROM StreamUsers WHERE stream_name = '%s' AND username = '%s';";
    char streamUserInsertion[MAX_QUERY];
    sprintf(streamUserInsertion, formatStr, streamName, username);
    
    MYSQL mysql;
    connectToDB(&mysql);

    mysql_query(&mysql, streamUserInsertion);

    MYSQL_RES *res;
    if ((res = mysql_store_result(&mysql)) == NULL){
        error("Failed to verify user belongs to stream.",&mysql);
    }

    mysql_close(&mysql);

    if (mysql_num_rows(res) == 0) {
        return 0;
    }

    return 1;
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
