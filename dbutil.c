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
#include <mysql/mysql.h>
#include "dbutil.h"

/* mysql -h HOSTNAME -u USERNAME -pPASSWORD */
#define HOSTNAME  "HOSTNAME"
#define USERNAME  "USERNAME"
#define PASSWORD  "PASSWORD"

#define DATABASE  "DATABASE"

/*
    Standard error clause.
*/
void error(char *msg, MYSQL *mysql) {
    printf("%s\n%s\n",msg,mysql_error(mysql));
    exit(1);
}

/*
    Creates connection to the database with initialized MYSQL variable.
*/
void connectToDB(MYSQL *mysql) {
    mysql_init(mysql);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
       error("Could not connect to host.",mysql);
    }
    return;
}