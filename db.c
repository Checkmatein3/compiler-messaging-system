/*
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 4
    March 31, 2017
    This Database Reset Program is used to view the data of or reset the database 
    for a message management and viewing system.
***********************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "dbutil.h"

/* Command strings */
#define CLEAR "-clear"
#define RESET "-reset"
#define POSTS "-posts"
#define USERS "-users"
#define STREAMS "-streams"
#define HELP "-help"
#define STREAMS_WITH_USER "-streamsWithUser"
#define TOTAL_POSTS_READ "-totalPostsRead"
#define UPDATE_LAST_POST_READ "-updateLastPostRead"
#define MARK_ALL_POSTS_READ "-markAllPostsRead"
#define POST_AT_INDEX "-postAtIndex"

/*
    Deletes the tables from the database.
*/
static void resetDB() {
    char dropStreamTablesQuery[MAX_QUERY];
    strcpy(dropStreamTablesQuery, "DROP TABLE IF EXISTS Stream, StreamUsers;");
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, dropStreamTablesQuery)) {
        error("Failed drop tables",&mysql);
    }

    mysql_close(&mysql);

    return;
}

/*
    Removes all of the posts, users, streams, and any other information
    from the tables in the database.
*/
static void clearDB() {
    char removeStreamQuery[MAX_QUERY];
    strcpy(removeStreamQuery, "DELETE FROM Stream;");
    char removeStreamUsersQuery[MAX_QUERY];
    strcpy(removeStreamUsersQuery, "DELETE FROM StreamUsers;");
    
    MYSQL mysql;
    connectToDB(&mysql);

    /* remove stream before stream users */

    if (mysql_query(&mysql, removeStreamQuery)) {
        error("Failed Stream remove",&mysql);
    }

    if (mysql_query(&mysql, removeStreamUsersQuery)) {
        error("Failed StreamUsers remove",&mysql);
    }

    mysql_close(&mysql);

    return;
}

/*
    Prints out all of the posts stored in the database.
*/
static void printPosts() {
    char selectPostsQuery[MAX_QUERY];
    strcpy(selectPostsQuery, "SELECT * FROM Stream;");
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, selectPostsQuery)) {
        error("Failed posts select",&mysql);
    }

    MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))){
        error("Fail posts store",&mysql);
    }

    MYSQL_ROW row;
    /*unsigned int numFields = mysql_num_fields(res);*/
    while ((row = mysql_fetch_row(res))) {
        printf("Sender: %s\n"
                "Date: %s\n"
                "%s",
                row[2], row[4], row[3]);
        printf("\n");
    }

    mysql_close(&mysql);

    return;
}

/*
    Prints out all of the stream names stored in the databse.
*/
static void printUsers() {
    char selectUsersQuery[MAX_QUERY];
    strcpy(selectUsersQuery, "SELECT DISTINCT username FROM StreamUsers;");
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, selectUsersQuery)) {
        error("Failed users select",&mysql);
    }

    MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))){
        error("Failed users store",&mysql);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        printf("%s\n", row[0]);
    }

    mysql_close(&mysql);

    return;
}

/*
    Prints out all of the streams stored in the database.
*/
static void printStreams() {
    char selectStreamsQuery[MAX_QUERY];
    strcpy(selectStreamsQuery, "SELECT DISTINCT stream_name FROM StreamUsers;");
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, selectStreamsQuery)) {
        error("Failed streams select",&mysql);
    }

    MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))){
        error("Failed streams store",&mysql);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        printf("%s\n", row[0]);
    }

    mysql_close(&mysql);

    return;
}

/*
    Prints out a usage message.
*/
static void printHelp() {
    printf("\nUsage: db <flag>\n\n"
            "List of all flags:\n"
            "%s\t%s\n"
            "%s\t%s\n"
            "%s\t%s\n"
            "%s\t%s\n"
            "%s\t%s\n"
            "%s\t%s\n",
            CLEAR, "Deletes the tables from the database.",
            RESET, "Removes all of the posts, users, streams, and any other information from the tables in the database.",
            POSTS, "Prints out all of the posts stored in the database.",
            USERS, "Prints out all of the stream names stored in the databse.",
            STREAMS, "Prints out all of the streams stored in the database.",
            HELP, "Prints out a usage message.");
    return;
}

static void printStreamsWithUser() {
    char username[MAX_USERNAME_LEN];

    if (fscanf(stdin, "%[^\n]\n", username) != 1) {
        printf("Invalid streamsWithUser input format\n");
        return;
    }

    char selectStreamsWithUserQuery[MAX_QUERY];
    sprintf(selectStreamsWithUserQuery, "SELECT stream_name FROM StreamUsers WHERE username = '%s';", username);
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, selectStreamsWithUserQuery)) {
        error("Failed streams select",&mysql);
    }

    MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))){
        error("Failed streams store",&mysql);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        printf("%s\n", row[0]);
    }

    mysql_close(&mysql);

    return;
}

static void printTotalPostsRead() {
    char streamName[MAX_STREAMNAME_LEN];
    char username[MAX_USERNAME_LEN];

    if (fscanf(stdin, "%[^\n]\n%[^\n]\n", streamName, username) != 2) {
        printf("Invalid totalPostsRead input format\n");
        return;
    }

    char selectTotalPostsReadQuery[MAX_QUERY];
    sprintf(selectTotalPostsReadQuery, 
        "SELECT SUM(last_post_read) FROM StreamUsers WHERE stream_name IN(%s) AND username = '%s';", 
        streamName, username);
/*printf("%s", selectTotalPostsReadQuery);*/
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, selectTotalPostsReadQuery)) {
        error("Failed streams select",&mysql);
    }

    MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))){
        error("Failed streams store",&mysql);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        printf("%s\n", row[0]);
    }

    mysql_close(&mysql);

    return;
}

static void updateLastPostRead() {
    char streamName[MAX_STREAMNAME_LEN];
    char username[MAX_USERNAME_LEN];
    int lastPostRead;

    if (fscanf(stdin, "%[^\n]\n%[^\n]\n%d\n", streamName, username, &lastPostRead) != 3) {
        printf("Invalid updateLastPostRead input format\n");
        return;
    }

    char updateLastPostReadQuery[MAX_QUERY];
    sprintf(updateLastPostReadQuery, 
        "UPDATE StreamUsers SET last_post_read = '%d' WHERE stream_name = '%s' AND username = '%s';", 
        lastPostRead, streamName, username);
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, updateLastPostReadQuery)) {
        error("Failed updateLastPostRead",&mysql);
    }

    mysql_close(&mysql);

    return;
}

static void markAllPostsRead() {
    char streamName[MAX_USERNAME_LEN];
    char username[MAX_USERNAME_LEN];

    if (fscanf(stdin, "%[^\n]\n%[^\n]\n", streamName, username) != 1) {
        printf("Invalid markAllPostsRead input format\n");
        return;
    }

    char markAllPostsReadQuery[MAX_QUERY];

    if (strcmp(streamName, "all") == 0) {
        sprintf(markAllPostsReadQuery, 
            "UPDATE StreamUsers t1 \n"
            "INNER JOIN (\n"
                "SELECT stream_name, COUNT(*) AS LP \n"
                "FROM Stream GROUP BY stream_name\n"
            ") AS t2 ON t1.stream_name = t2.stream_name \n"
            "AND t1.stream_name = t2.stream_name\n"
            "SET last_post_read = t2.LP\n"
            "WHERE username = '%s';\n", 
            username);
    }
    else {
        sprintf(markAllPostsReadQuery, 
        "UPDATE StreamUsers t1 \n"
        "INNER JOIN (\n"
            "SELECT stream_name, COUNT(*) AS LP \n"
            "FROM Stream GROUP BY stream_name\n"
        ") AS t2 ON t1.stream_name = t2.stream_name \n"
        "AND t1.stream_name = t2.stream_name\n"
        "SET last_post_read = t2.LP\n"
        "WHERE username = '%s'"
        "AND stream_name = '%s';\n", 
        username, streamName);
    }   

    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, markAllPostsReadQuery)) {
        error("Failed streams select",&mysql);
    }

    mysql_close(&mysql);

    return;
}

static void printPostAtIndex() {
    char streamName[MAX_STREAMNAME_LEN];
    int postIndex;

    if (fscanf(stdin, "%[^\n]\n%d\n", streamName, &postIndex) != 2) {
        printf("Invalid postAtIndex input format\n");
        return;
    }

    char selectPostsAtIndexQuery[MAX_QUERY];
    sprintf(selectPostsAtIndexQuery, "SELECT * FROM Stream WHERE stream_name = '%s';", streamName);
    
    MYSQL mysql;
    connectToDB(&mysql);

    if (mysql_query(&mysql, selectPostsAtIndexQuery)) {
        error("Failed streams select",&mysql);
    }

    MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))){
        error("Failed streams store",&mysql);
    }

    if ((mysql_num_rows(res) - 1 >= postIndex)) {
        MYSQL_ROW row;
        mysql_data_seek(res, postIndex);
        if ((row = mysql_fetch_row(res))) {
            printf("Sender: %s\n"
                    "Date: %s\n"
                    "%s",
                    row[2], row[4], row[3]);
        }
    }

    mysql_close(&mysql);

    return;
}

int main(int argc, char const *argv[]) {
    if (argc == 2) {
        /* Choose command */
        if (strcmp(argv[1], CLEAR) == 0) {
            resetDB();
        }
        else if (strcmp(argv[1], RESET) == 0) {
            clearDB();
        }
        else if (strcmp(argv[1], POSTS) == 0) {
            printPosts();
        }
        else if (strcmp(argv[1], USERS) == 0) {
            printUsers();
        }
        else if (strcmp(argv[1], STREAMS) == 0) {
            printStreams();
        }
        else if (strcmp(argv[1], HELP) == 0) {
            printHelp();
        }
        else if (strcmp(argv[1], STREAMS_WITH_USER) == 0) {
            printStreamsWithUser();
        }
        else if (strcmp(argv[1], TOTAL_POSTS_READ) == 0) {
            printTotalPostsRead();
        }
        else if (strcmp(argv[1], UPDATE_LAST_POST_READ) == 0) {
            updateLastPostRead();
        }
        else if (strcmp(argv[1], MARK_ALL_POSTS_READ) == 0) {
            markAllPostsRead();
        }
        else if (strcmp(argv[1], POST_AT_INDEX) == 0) {
            printPostAtIndex();
        }
        else {
            printf("Invalid command.\n");
            printHelp();
        }
    }
    else {
        printf("Invalid command.\n");
        printHelp();
    }
    return 0;
}
