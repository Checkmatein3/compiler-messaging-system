#!/usr/bin/python3

'''
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 3
    March 17,2017
    This program is the interface for a web application that allows for retrieving 
    messages from streams of a message management and viewing system.
***********************************************************************************
'''

from os import walk
from sys import argv,exit#,path
from json import loads
from datetime import datetime
from subprocess import Popen,PIPE
#path.append('./a2src')

#from view import getUsername,getStreamsWithUsername

#
#   stream files
#

# def getStreamFileNames():
#     #get the files in the messages directory
#     messageFiles = []
#     for (root, directoryNames, fileNames) in walk("messages/"):
#         messageFiles.extend(fileNames)
#         break
#     return messageFiles

# def createStreamUsersFileName(streamName):
#     return ('messages/' + streamName + 'StreamUsers')

# def createStreamMessagesFileName(streamName):
#     return ('messages/' + streamName + 'Stream')

# def createStreamDataFileName(streamName):
#     return ('messages/' + streamName + 'StreamData')

#
#   streams
#

# def getStreamNames():
#     #get the streamNames from the user files in the messages folder
#     messageFiles = getStreamFileNames()
#     streamNames = []
#     for messageFile in messageFiles:
#         #check that it is a stream user file
#         if messageFile[len(messageFile) - 11:] == "StreamUsers":
#             streamNames.append(messageFile[:len(messageFile) - 11])
#     return streamNames

def getStreamNames():
    try:
        process = Popen(["./db", "-streams"], universal_newlines=True, stdin=PIPE, stdout=PIPE)
        streamNames = process.communicate()[0].split()
        process.wait()
        if (process.returncode != 0):
            return ()
        return streamNames
    except OSError as e:
        return ()

def printStreamNames():
    streamNames = getStreamNames()
    for streamName in streamNames:
        print(streamName)
    return 0

#
#   users
#

def getUsername(usernameTokens):
    username = " ".join(usernameTokens)
    return username

# def getStreamsWithUsername(username):
#     #get the streamNames of streams containing the user
#     messageFiles = getStreamFileNames()
#     streamNames = []
#     for messageFile in messageFiles:
#         #check that it is a stream user file
#         if messageFile[len(messageFile) - 11:] == "StreamUsers":
#             streamUserFile = open("messages/" + messageFile, "r")
#             streamUserFileLines = streamUserFile.read().splitlines()
            
#             #check if the stream user file contains the user
#             for line in streamUserFileLines:
#                 userTokens = line.split()[:-1]
#                 currentUsername = " ".join(userTokens)
#                 if currentUsername == username:
#                     streamNames.append(messageFile[:len(messageFile) - 11])
#                     break
#             streamUserFile.close()
#     return streamNames

def getStreamsWithUsername(username):
    try:
        process = Popen(["./db", "-streamsWithUser"], universal_newlines=True, stdin=PIPE, stdout=PIPE)
        streamNames = process.communicate(input=username)[0].splitlines()
        process.wait()
        if (process.returncode != 0):
            return ()
#        print(streamNames)
        return streamNames
    except OSError as e:
        return ()

def printStreamsWithUsername(username):
    streamsWithUsername = getStreamsWithUsername(username)
    for stream in streamsWithUsername:
        print(stream)
    return 0

# def getNumPostsRead(streamName, username):
#     try:
#         streamUsersFileName = createStreamUsersFileName(streamName)
#         streamUsersFile = open(streamUsersFileName, "r")
#         streamUserFileLines = streamUsersFile.read().splitlines()
        
#         #update the last post read for the stream by the user
#         for line in streamUserFileLines:
#             userTokens = line.split()[:-1]
#             currentUsername = " ".join(userTokens)
#             if currentUsername == username:
#                 #the index of the post is one less than 
#                 #the number of posts read by the author
#                 lastPostReadIndex = int(line.split()[-1]) - 1
#                 return lastPostReadIndex
#     except IOError:
#         return None

# def getTotalPostsRead(streamNames, username):
#     totalPostsRead = 0
#     for streamName in streamNames:
#         try:
#             streamUsersFileName = createStreamUsersFileName(streamName)
#             streamUsersFile = open(streamUsersFileName, "r")
#             streamUserFileLines = streamUsersFile.read().splitlines()
            
#             for line in streamUserFileLines:
#                 userTokens = line.split()[:-1]
#                 currentUsername = " ".join(userTokens)
#                 if currentUsername == username:
#                     #the number of posts read by the author for current stream
#                     totalPostsRead += int(line.split()[-1])
#         except IOError:
#             return 0
#     return totalPostsRead

def getTotalPostsRead(streamNames, username):
    try:
        process = Popen(["./db", "-totalPostsRead"], universal_newlines=True, stdin=PIPE, stdout=PIPE)
        streamNamesToQuery = "'"+"','".join(streamNames)+"'"
#        print(streamNames)
#        print(streamNamesToQuery)
        totalPostsRead = int(process.communicate(input=streamNamesToQuery+"\n"+username)[0])
        process.wait()
        if (process.returncode != 0):
            return 0
        return totalPostsRead
    except OSError as e:
        return 0

# def updateUsersLastPostRead(streamName, username, numPostsRead):
#     try:
#         streamUsersFileName = createStreamUsersFileName(streamName)
#         streamUsersFile = open(streamUsersFileName, "r+")
#         streamUserFileLines = streamUsersFile.read().splitlines()
        
#         #update the last post read for the stream by the user
#         streamUsersFile.seek(0)
#         streamUsersFile.truncate()
#         for line in streamUserFileLines:
#             userTokens = line.split()[:-1]
#             currentUsername = " ".join(userTokens)
#             if currentUsername == username:
#                 if numPostsRead > int(line.split()[-1]):
#                     line = currentUsername + " " + str(numPostsRead)
#             streamUsersFile.write(line + "\n")
#         streamUsersFile.close()
#     except IOError:
#         None

def updateUsersLastPostRead(streamName, username, numPostsRead):
    lastPostReadIndex = getTotalPostsRead([streamName], username) - 1
    if numPostsRead > lastPostReadIndex:
        try:
            process = Popen(["./db", "-updateLastPostRead"], universal_newlines=True, stdin=PIPE, stdout=PIPE)
            process.communicate(input=streamName+'\n'+username+'\n'+str(numPostsRead)+'\n')
            process.wait()
            return
        except OSError as e:
            return


# def markAllPostsAsRead(streamToMarkAsRead, username):
#     if streamToMarkAsRead == "all":
#         streamsToMarkAsRead = getStreamsWithUsername(username)
#     else:
#         streamsToMarkAsRead = [streamToMarkAsRead]
#     for streamName in streamsToMarkAsRead:
#         try:
#             streamDataFileName = createStreamDataFileName(streamName)
#             streamDataFile = open(streamDataFileName, "r")
#             numPostsRead = len(streamDataFile.read().splitlines())
#             updateUsersLastPostRead(streamName, username, numPostsRead)
#         except IOError:
#             return 1
#     return 0

def markAllPostsAsRead(streamToMarkAsRead, username):
    try:
        process = Popen(["./db", "-markAllPostsRead"], universal_newlines=True, stdin=PIPE, stdout=PIPE)
        # streamNamesToQuery = "'"+"','".join(streamNames)+"'"
        process.communicate(input=streamToMarkAsRead+'\n'+username+'\n')
        process.wait()
        return
    except OSError as e:
        return

#
#   posts
#

# def getDate(postMessage):
#     for line in postMessage.splitlines():
#         if line.find('Date: ') >= 0:
#             dateStr = line[6:]
#             return datetime.strptime(dateStr, '%a %b %d %X %Y')
#     return

def getDate(postMessage):
    for line in postMessage.splitlines():
        if line.find('Date: ') >= 0:
            dateStr = line[6:]
            return datetime.strptime(dateStr, '%Y-%m-%d %H:%M:%S')
    return

def getSender(postMessage):
    for line in postMessage.splitlines():
        if line.find('Sender: ') >= 0:
            username = line[8:]
            return username
    return

# def getPostAtIndex(streamName, postIndex):
#     if streamName is None:
#         return None

#     streamDataFileName = createStreamDataFileName(streamName)
#     try:
#         streamDataFile = open(streamDataFileName, "r")
#         streamDataFileLines = streamDataFile.read().splitlines()
#         streamDataFile.close()
#     except IOError:
#         return None

#     #byte positons
#     try:
#         if postIndex > 0:
#             postStartBytePos = int(streamDataFileLines[postIndex - 1])
#         elif postIndex == 0:
#             postStartBytePos = 0
#         else:
#             return None
#         postEndBytePos = int(streamDataFileLines[postIndex])
#     except IndexError:
#         return None

#     #get the next message for the stream by date
#     streamMessageFileName = createStreamMessagesFileName(streamName)
#     try:
#         streamMessageFile = open(streamMessageFileName, "r")
#         streamMessageFile.seek(postStartBytePos, 0)
#         byteDifference = postEndBytePos - postStartBytePos
#         postMessage = streamMessageFile.read(byteDifference)
#         streamMessageFile.close()
        
#         date = getDate(postMessage)
#         author = getSender(postMessage)
#         return {(streamName, postIndex):{"date":date, "author":author, "message":postMessage}}
#     except IOError:
#         return None

def getPostAtIndex(streamName, postIndex):
    try:
        process = Popen(["./db", "-postAtIndex"], universal_newlines=True, stdin=PIPE, stdout=PIPE)
        postMessage = process.communicate(input=streamName+'\n'+str(postIndex))[0]
        process.wait()
        if (process.returncode != 0):
            return None
        date = getDate(postMessage)
        if (date is None):
            return None
        author = getSender(postMessage)
        return {(streamName, postIndex):{"date":date, "author":author, "message":postMessage}}
    except OSError as e:
        return None

def getPosts(streamName):
    posts = []
    postIndex = 0;
    post = getPostAtIndex(streamName, postIndex)
    while post is not None:
    # while (post = getPostAtIndex(streamName, postIndex)) is not None:
        posts.append(post)
        postIndex += 1
        post = getPostAtIndex(streamName, postIndex)
    return posts

def getAllPosts(streamNames):
    allPosts = []
    for streamName in streamNames:
        streamPosts = getPosts(streamName)
        allPosts.extend(streamPosts)
    return allPosts


def getNewPost(offset, sortMode, currentPostIndex, currentStream, username, streamsToView):
    allPosts = getAllPosts(streamsToView)

    if sortMode == "sortByDate":
        allPosts = sorted(allPosts, key=lambda post: post[(list(post.keys())[0][0],list(post.keys())[0][1])]['date'])
    elif sortMode == "sortByUsername":
        allPosts = sorted(allPosts, key=lambda post: post[(list(post.keys())[0][0],list(post.keys())[0][1])]['author'])
    else:
        return None

    if currentStream == 'NO_STREAM':
        totalNumPostsRead = getTotalPostsRead(streamsToView, username)
        newPostListIndex = totalNumPostsRead + offset - 1
    else:   
        newPostListIndex = currentPostIndex + offset

    totalNumPosts = len(allPosts)

    if newPostListIndex >= totalNumPosts:
        if currentStream == 'NO_STREAM':
            nextPost = allPosts[totalNumPosts - 1]
            newPostKey = list(nextPost.keys())[0]
            currentStream = newPostKey[0]
        return {(currentStream,totalNumPosts):{"date":None,"author":None,"message":"No newer posts."}}
    elif newPostListIndex < 0:
        if currentStream == 'NO_STREAM':
            nextPost = allPosts[0]
            newPostKey = list(nextPost.keys())[0]
            currentStream = newPostKey[0]
        return {(currentStream,-1):{"date":None,"author":None,"message":"No older posts."}}
    else:        
        newPost = allPosts[newPostListIndex]
        newPostKey = list(newPost.keys())[0]
        newStream = newPostKey[0]
        newPostStreamIndex = newPostKey[1]
        
        if sortMode == "sortByDate":
            updateUsersLastPostRead(newStream, username, newPostStreamIndex + 1)
        
        newPost[(newStream, newPostListIndex)] = newPost.pop(newPostKey)
#print(newPost)
    return newPost

def printNextPost(sortMode, currentPostIndex, currentStream, username, streamToView):
    if streamToView == "all":
        streamsToView = getStreamsWithUsername(username)
    else:
        streamsToView = [streamToView]
    nextPost = getNewPost(1, sortMode, int(currentPostIndex), currentStream, username, streamsToView)
    if nextPost is None:
        print("Invalid sort mode.")
        return 1

    nextPostKey = list(nextPost.keys())[0]
    nextStream = nextPostKey[0]
    nextPostIndex = nextPostKey[1]
    postMessage = list(nextPost.values())[0]['message']
       
    print(nextStream)
    print(nextPostIndex)
    print(postMessage)

def printPrevPost(sortMode, currentPostIndex, currentStream, username, streamToView):
    if streamToView == "all":
        streamsToView = getStreamsWithUsername(username)
    else:
        streamsToView = [streamToView]
    nextPost = getNewPost(-1, sortMode, int(currentPostIndex), currentStream, username, streamsToView)
    if nextPost is None:
        print("Invalid sort mode.")
        return 1

    nextPostKey = list(nextPost.keys())[0]
    nextStream = nextPostKey[0]
    nextPostIndex = nextPostKey[1]
    postMessage = list(nextPost.values())[0]['message']
       
    print(nextStream)
    print(nextPostIndex)
    print(postMessage)

def printCurrentPost(sortMode, currentPostIndex, currentStream, username, streamToView):
    if streamToView == "all":
        streamsToView = getStreamsWithUsername(username)
    else:
        streamsToView = [streamToView]
#        print(streamsToView)
#        print("hi")
    nextPost = getNewPost(0, sortMode, int(currentPostIndex), currentStream, username, streamsToView)
    if nextPost is None:
        print("Invalid sort mode.")
        return 1

    nextPostKey = list(nextPost.keys())[0]
    nextStream = nextPostKey[0]
    nextPostIndex = nextPostKey[1]
    postMessage = list(nextPost.values())[0]['message']

    print(nextStream)
    print(nextPostIndex)
    print(postMessage)

if __name__ == '__main__':
    functions = { 
                    "printStreamsWithUsername":printStreamsWithUsername, 
                    "printStreamNames":printStreamNames,
                    "markAllPostsAsRead":markAllPostsAsRead,
                    "printNextPost":printNextPost,
                    "printPrevPost":printPrevPost,
                    "printCurrentPost":printCurrentPost
                }
    status = 1

    if len(argv) >= 2:
        functionName = argv[1]
        if functionName == "printStreamsWithUsername":
            status = functions["printStreamsWithUsername"](getUsername(argv[2:]))
        elif functionName == "printStreamNames":
            status = functions["printStreamNames"]()
        elif functionName == "markAllPostsAsRead":
            endArgs = " ".join(argv[2:]).split("+")
            status = functions["markAllPostsAsRead"](endArgs[0], endArgs[1])
        elif functionName == "printNextPost":
            endArgs = " ".join(argv[4:]).split("+")
            status = functions["printNextPost"](argv[2], argv[3], endArgs[0], endArgs[1], endArgs[2])
        elif functionName == "printPrevPost":
            endArgs = " ".join(argv[4:]).split("+")
            status = functions["printPrevPost"](argv[2], argv[3], endArgs[0], endArgs[1], endArgs[2])
        elif functionName == "printCurrentPost":
            endArgs = " ".join(argv[4:]).split("+")
            status = functions["printCurrentPost"](argv[2], argv[3], endArgs[0], endArgs[1], endArgs[2])
    exit(status)
