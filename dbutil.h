/*
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 4
    March 31, 2017
    This Database Reset Program is used to view the data of or reset the database 
    for a message management and viewing system.
***********************************************************************************
*/

#ifndef __DB_H__
#define __DB_H__

#include <mysql/mysql.h>

#define MAX_USERNAME_LEN 256
#define MAX_STREAM_LIST_LEN 1024
#define MAX_STREAMNAME_LEN 256
#define MAX_DATE_LEN 26
#define MAX_MESSAGE_LINE_LEN 256
#define MAX_TEXT_LEN 5000
#define MAX_QUERY 8000

void error(char *msg, MYSQL *mysql);

void connectToDB(MYSQL *mysql);

#endif
