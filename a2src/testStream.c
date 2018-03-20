/*
***********************************************************************************
    Daniel Kolodziejczak

***********************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include "stream.h"

void testAddUser() {
    addUser("bob", "testStream1");
    printf("Add User 1 Complete:");
    while (getchar() != '\n');

    addUser("bob", "testStream1");
    printf("Add User 2 Complete:");
    while (getchar() != '\n');

    addUser("bob", "testStream2");
    printf("Add User 3 Complete:");
    while (getchar() != '\n');

    addUser("bob", "testStream3,testStream4");
    printf("Add User 4 Complete:");
    while (getchar() != '\n');

    addUser("bob", "testStream3,testStream4");
    printf("Add User 5 Complete:");
    while (getchar() != '\n');

    addUser("jim", "testStream1");
    printf("Add User 6 Complete:");
    while (getchar() != '\n');

    addUser("jim", "testStream2,testStream3");
    printf("Add User 7 Complete:");
    while (getchar() != '\n');

    addUser("jim bob joe", "testStream1");
    printf("Add User 8 Complete:");
    while (getchar() != '\n');
}

void testRemoveUser() {
    removeUser("jim bob joe", "testStream1");
    printf("Remove User 8 Complete:");
    while (getchar() != '\n');

    removeUser("jim", "testStream2,testStream3");
    printf("Remove User 7 Complete:");
    while (getchar() != '\n');

    removeUser("jim", "testStream1");
    printf("Remove User 6 Complete:");
    while (getchar() != '\n');

    removeUser("bob", "testStream3,testStream4");
    printf("Remove User 5 Complete:");
    while (getchar() != '\n');

    removeUser("bob", "testStream3,testStream4");
    printf("Remove User 4 Complete:");
    while (getchar() != '\n');

    removeUser("bob", "testStream2");
    printf("Remove User 3 Complete:");
    while (getchar() != '\n');

    removeUser("bob", "testStream1");
    printf("Remove User 2 Complete:");
    while (getchar() != '\n');

    removeUser("bob", "testStream1");
    printf("Remove User 1 Complete:");
    while (getchar() != '\n');
}

void testUpdateStream() {
    struct userPost *post = malloc(sizeof(struct userPost));
    int failureUpdatingStream, i = 1;
    post->text = "text...\ntext...\n";

    printf("Testing valid postings...\n");
    /*Stream 1*/
    /*first post to a new stream*/
    post->username = "user1";
    post->streamname = "messages/stream1";
    post->date = "post1";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (!failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*second post to an existing stream by a different poster than the first poster*/
    post->username = "user2";
    post->streamname = "messages/stream1";
    post->date = "post2";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (!failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*third post to an existing stream by the same poster as the last poster*/
    post->username = "user1";
    post->streamname = "messages/stream1";
    post->date = "post3";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (!failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*Stream 2*/
    /*first post to a new stream*/
    post->username = "user1";
    post->streamname = "messages/stream2";
    post->date = "post1";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (!failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*second post to an existing stream by the same poster as the first poster*/
    post->username = "user1";
    post->streamname = "messages/stream2";
    post->date = "post2";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (!failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*third post to an existing stream by a different poster than the last poster*/
    post->username = "user2";
    post->streamname = "messages/stream2";
    post->date = "post3";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (!failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*Invalid post data*/
    printf("\nTesting invalid postings...\n");

    /*username is an empty string*/
    post->username = "";
    post->streamname = "messages/stream1";
    post->date = "date";
    post->text = "text...\ntext...\n";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*streamname is an empty string*/
    post->username = "user1";
    post->streamname = "";
    post->date = "date";
    post->text = "text...\ntext...\n";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*date is an empty string*/
    post->username = "user1";
    post->streamname = "messages/stream1";
    post->date = "";
    post->text = "text...\ntext...\n";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*date is an empty string*/
    post->username = "user1";
    post->streamname = "messages/stream2";
    post->date = "post4";
    post->text = "";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*text is NULL*/
    post->username = "user1";
    post->streamname = "messages/stream1";
    post->date = "date";
    post->text = NULL;
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*date is NULL*/
    post->username = "user1";
    post->streamname = "messages/stream1";
    post->date = NULL;
    post->text = "text...\ntext...\n";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*streamName is NULL*/
    post->username = "user1";
    post->streamname = NULL;
    post->date = "date";
    post->text = "text...\ntext...\n";
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*userName is NULL*/
    post->username = NULL;
    post->streamname = NULL;
    post->date = NULL;
    post->text = NULL;
    failureUpdatingStream = updateStream(post);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;

    /*userPost is NULL*/
    failureUpdatingStream = updateStream(NULL);
    printf("Test %d: ", i);
    if (failureUpdatingStream) {
        printf("PASS\n");
    }
    else {
        printf("FAIL\n");
    }
    i++;
    free(post);
}

/*
    
    Pre:    0: test addUser; 1: test removeUser; 2: test updateStream
    Post:   
*/
int main(int argc, char** argv) {
    char selection = argv[1][0];
    switch(selection) {
        case '0':
            testAddUser();
            break;
        case '1':
            testRemoveUser();
            break;
        case '2':
            testUpdateStream();
            break;
        default:
            printf("Invalid test selected.\n");
            break;
    }
    printf("\nTesting Complete.\n");
    return 0;
}