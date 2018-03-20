#!/usr/bin/python3

'''
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 2
    February 17,2017
    This program allows users to view messages from streams of a message management 
    and viewing system.
***********************************************************************************
'''

from sys import argv
from os import walk
from datetime import datetime

sortModes = ('date', 'username')
postTypes = ('next', 'prev')

def getUsername(usernameTokens):
    username = " ".join(usernameTokens)
    return username

def getStreamsWithUsername(username):
    #get the streamnames of streams containing the user
    messageFiles = getMessageFiles()
    streamnames = []
    for messageFile in messageFiles:
        #check that it is a stream user file
        if messageFile[len(messageFile) - 11:] == "StreamUsers":
            streamUserFile = open("messages/" + messageFile, "r")
            streamUserFileLines = streamUserFile.read().splitlines()
            
            #check if the stream user file contains the user
            for line in streamUserFileLines:
                userTokens = line.split()[:-1]
                currentUsername = " ".join(userTokens)
                if currentUsername == username:
                    streamnames.append(messageFile[:len(messageFile) - 11])
                    break
            streamUserFile.close()
    return streamnames

def getMessageFiles():
    #get the files in the message directory
    messageFiles = []
    for (root, directoryNames, fileNames) in walk("messages/"):
        messageFiles.extend(fileNames)
        break
    return messageFiles

def getStreamsToView(streamnames):
    streamname = input("    ".join(streamnames) + "\n")
    if streamname == "all":
        return streamnames
    elif streamname in streamnames:
        return [streamname]
    else:
        return None

def getstreamsPrevPostRead(streamsToView, username):
    streamsPrevPostRead = {}
    for streamname in streamsToView:
        try:
            streamUsersFileName = createStreamUsersFileName(streamname)
            streamUsersFile = open(streamUsersFileName, "r")
            streamUserFileLines = streamUsersFile.read().splitlines()
            
            #update the last post read for the stream by the user
            for line in streamUserFileLines:
                userTokens = line.split()[:-1]
                currentUsername = " ".join(userTokens)
                if currentUsername == username:
                    lastPostReadIndex = int(line.split()[-1]) - 1
                    streamsPrevPostRead[streamname] = lastPostReadIndex
                    break
        except IOError:
            return None
    return streamsPrevPostRead

def viewStreams(streamsPrevPostRead, username):
    pagedown = b'\x1b[6~'
    pageup = b'\x1b[5~'
    sortMode = sortModes[0]

    samePostCount = [0]
    prevPostRead = None

    #end of display
    for i in range(0, 23):
        print()
    print('Page Up  Page Down  O-order toggle  M-mark all  S-stream  C-check for new')
    displayPosts(sortMode, postTypes[0], streamsPrevPostRead, username, samePostCount, prevPostRead)

    while True:
        selection = input()
        selectionUTF8 = selection.encode()
        selectionLower = selection.lower()

        if selectionUTF8 == pagedown:
            prevPostRead = displayPosts(sortMode, postTypes[0], streamsPrevPostRead, username, samePostCount, prevPostRead)
        elif selectionUTF8 == pageup:
            prevPostRead = displayPosts(sortMode, postTypes[1], streamsPrevPostRead, username, samePostCount, prevPostRead)
        elif selectionLower == 'o': 
            sortMode = toggleSort(sortModes, sortMode)
            prevPostRead = None
        elif selectionLower == 'm': 
            markAllAsRead(streamsPrevPostRead.keys(), username);
        elif selectionLower == 'c': 
            prevPostRead = checkForNew(sortMode, postTypes[0], streamsPrevPostRead, username, samePostCount, prevPostRead)
        elif selectionLower == 's':
            return True
        elif selectionLower == 'q':
            return False

def toggleSort(sortModes, sortMode):
    sortMode = sortModes[(sortModes.index(sortMode) + 1) % len(sortModes)]
    return sortMode

def markAllAsRead(streamnames, username):
    for streamname in streamnames:
        try:
            streamDataFileName = createStreamDataFileName(streamname)
            streamDataFile = open(streamDataFileName, "r")
            lastPostIndex = len(streamDataFile.read().splitlines()) - 1
            updateStreamUsers(streamname, username, lastPostIndex)
        except IOError:
            None
    return

def checkForNew(sortMode, postType, streamsPrevPostRead, username, samePostCount, prevPostRead):
    #displayPosts already checks for new posts
    return displayPosts(sortMode, postType, streamsPrevPostRead, username, samePostCount, prevPostRead)

def displayPosts(sortMode, postType, streamsPrevPostRead, 
    username, samePostCount, prevPostRead):
    #get the next posts to display
    MAX_NUM_LINES = 24
    postsToDisplay = getNextPostsToDisplay(sortMode, postType, streamsPrevPostRead,
                        username, samePostCount, prevPostRead, MAX_NUM_LINES)
    
    numLinesPrinted = 0
    date = None
    if postsToDisplay is None or postsToDisplay == {} or postsToDisplay.keys() == []:
        print("No more messages to display.")
        return
    else:
        #print the lines of each stream specified
        for date in sorted(postsToDisplay.keys()):
            message = postsToDisplay[date][0][1]
            messageLines = message.splitlines()
            linesRead = postsToDisplay[date][1]
            linesToRead = postsToDisplay[date][2]
            for i in range(linesRead, linesRead + linesToRead):
                print(messageLines[i])
                numLinesPrinted += 1

    #end of display
    for i in range(numLinesPrinted, MAX_NUM_LINES - 1):
        print()
    print('Page Up  Page Down  O-order toggle  M-mark all  S-stream  C-check for new')

    if date is None:
        return None
    else:
        streamname = postsToDisplay[date][0][0]
        postToDisplayNum = postsToDisplay[date][0][2]
        streamsPrevPostRead[streamname] = postToDisplayNum
        return postsToDisplay[date][0]

def getNextPostsToDisplay(sortMode, postType, streamsPrevPostRead, username, samePostCount, prevPostRead, MAX_NUM_LINES):
    totalNumLines = 0
    postsToDisplay = {}
    postToDisplay = None
    while totalNumLines != MAX_NUM_LINES - 1:
        #get the next post
        # if samePostCount[0] > 0:
        #     #print(samePostCount)
        #     if prevPostRead is None:
        #         return None
        #     datedPostToDisplay = getPost(prevPostRead, streamsPrevPostRead[prevPostRead])
        if sortMode == sortModes[0] and postType == postTypes[0]:
            datedPostToDisplay = getNextPostByDate(streamsPrevPostRead, username)
        elif sortMode == sortModes[1] and postType == postTypes[0]:
            datedPostToDisplay = getNextPostByUsername(streamsPrevPostRead, username)
        elif sortMode == sortModes[0] and postType == postTypes[1]:
            datedPostToDisplay = getPrevPostByDate(streamsPrevPostRead, username)
        elif sortMode == sortModes[1] and postType == postTypes[1]:
            datedPostToDisplay = getPrevPostByUsername(streamsPrevPostRead, username)
        #print datedPostToDisplay[]
        #no more posts to display
        if datedPostToDisplay is None:
            # if sortMode == sortModes[0] and postType == postTypes[0]:
            #     streamsPrevPostRead[streamname] += 1
            # elif sortMode == sortModes[0] and postType == postTypes[1]:
            #     streamsPrevPostRead[streamname] += 1
            break
        postToDisplay = datedPostToDisplay[0]
        if postToDisplay == prevPostRead:
            samePostCount[0] += 1
        else:
            samePostCount[0] = 0

        message = postToDisplay[1]
        # print("\nmessage\n" + message)
        linesRead = samePostCount[0] * (MAX_NUM_LINES - 1)
        linesInMessage = message.splitlines()
        numLinesInMessage = len(linesInMessage)
        # print("len: " + str(numLinesInMessage))
        linesToRead = len(message.splitlines()) - linesRead
        linesLeft = (MAX_NUM_LINES - 1) - totalNumLines
        # print("lines" + str(linesToRead) + " " + str(linesLeft))
        #check if their are enough lines to display the message
        if linesToRead > linesLeft:
            if postsToDisplay == {}:
                linesToRead = MAX_NUM_LINES - 1
                #samePostCount[0] += 1
                # print("whut")
            else:
                #postToDisplay = postsToDisplay[date][0]
                #samePostCount[0] = 0
                # print("hi")
                break
            #break
        elif linesToRead <= 0:
            samePostCount[0] = 0
            # #update the last post read by the stream
            # streamname = postsToDisplay[date][0][0]
            # postToDisplayNum = postsToDisplay[date][0][2]
            # streamsPrevPostRead[streamname] = postToDisplayNum
            continue
        totalNumLines += linesToRead

        #save the post to be displayed
        date = datedPostToDisplay[1]
        postsToDisplay[date] = [postToDisplay, linesRead, linesToRead]
        streamname = postToDisplay[0]
        postToDisplayNum = postToDisplay[2]
        updateStreamUsers(streamname, username, postToDisplayNum)

        # #update the last post read by the stream
        streamname = postsToDisplay[date][0][0]
        postToDisplayNum = postsToDisplay[date][0][2]
        if sortMode == sortModes[0] and postType == postTypes[0]\
            and streamsPrevPostRead[streamname] < postToDisplayNum:
            streamsPrevPostRead[streamname] = postToDisplayNum
        elif sortMode == sortModes[0] and postType == postTypes[1]\
            and streamsPrevPostRead[streamname] > postToDisplayNum:
            streamsPrevPostRead[streamname] = postToDisplayNum

    # #the last post read for the stream is updated
    # if postToDisplay is not None:
    #     streamname = postToDisplay[0]
    #     postToDisplayNum = postToDisplay[2]
    #     streamsPrevPostRead[streamname] = postToDisplayNum
    return postsToDisplay

def getPost(streamname, postNum):
    if streamname is None:
        return None

    streamDataFileName = createStreamDataFileName(streamname)
    try:
        streamDataFile = open(streamDataFileName, "r")
        streamDataFileLines = streamDataFile.read().splitlines()
        streamDataFile.close()
    except IOError:
        return None

    #byte positons
    try:
        if postNum > 0:
            nextPostStartBytePos = int(streamDataFileLines[postNum - 1])
        else:
            nextPostStartBytePos = 0
        nextPostEndBytePos = int(streamDataFileLines[postNum])
    except IndexError:
        return None

    #get the next message for the stream by date
    streamMessageFileName = createStreamMessagesFileName(streamname)
    try:
        streamMessageFile = open(streamMessageFileName, "r")
        streamMessageFile.seek(nextPostStartBytePos, 0)
        byteDifference = nextPostEndBytePos - nextPostStartBytePos
        followingMessage = streamMessageFile.read(byteDifference)
        streamMessageFile.close()
        
        date = getDate(followingMessage)
        #sender = getSender(followingMessage)
        return [[streamname, followingMessage, postNum], date]
    except IOError:
        return None

def getNextPostByDate(streamsPrevPostRead, username):
    datedMessages = {}
    for streamname in streamsPrevPostRead.keys():
        #get the next post
        numPostsRead = streamsPrevPostRead[streamname] + 1
        datedNextPost = getPost(streamname, numPostsRead);
        if datedNextPost is None:
            continue;

        date = datedNextPost[1]
        #datedNextPost[0][2] += 1
        datedMessages[date] = datedNextPost[0]


    #get the next message from all the streams by earliest date
    try:
        dates = datedMessages.keys()
        firstDate = sorted(dates)[0]
        postToDisplay = datedMessages[firstDate]
    except IndexError:
        return None
    return [postToDisplay, firstDate]

def getNextPostByUsername(streamsPrevPostRead, username):
    return None

def getPrevPostByDate(streamsPrevPostRead, username):
    datedMessages = {}
    for streamname in streamsPrevPostRead.keys():
        #get the prev post
        numPostsRead = streamsPrevPostRead[streamname] - 1
        if numPostsRead < 0:
            return None
        datedNextPost = getPost(streamname, numPostsRead);
        if datedNextPost is None:
            continue;

        date = datedNextPost[1]
        # datedNextPost[0][2] < 0
        datedMessages[date] = datedNextPost[0]


    #get the next message from all the streams by earliest date
    try:
        dates = datedMessages.keys()
        firstDate = sorted(dates, reverse = True)[0]
        postToDisplay = datedMessages[firstDate]
    except IndexError:
        return None
    return [postToDisplay, firstDate]

def getPrevPostByUsername(streamsPrevPostRead, username):
    return None

def updateStreamUsers(streamname, username, lastPostReadIndex):
    try:
        streamUsersFileName = createStreamUsersFileName(streamname)
        streamUsersFile = open(streamUsersFileName, "r+")
        streamUserFileLines = streamUsersFile.read().splitlines()
        
        #update the last post read for the stream by the user
        streamUsersFile.seek(0)
        streamUsersFile.truncate()
        for line in streamUserFileLines:
            userTokens = line.split()[:-1]
            currentUsername = " ".join(userTokens)
            if currentUsername == username:
                if lastPostReadIndex + 1 > int(line.split()[-1]):
                    line = currentUsername + " " + str(lastPostReadIndex + 1)
            streamUsersFile.write(line + "\n")
        streamUsersFile.close()
    except IOError:
        None

def getDate(message):
    for line in message.splitlines():
        if line.find('Date: ') >= 0:
            dateStr = line[6:]
            return datetime.strptime(dateStr, '%a %b %d %X %Y')
    return

def getSender(message):
    for line in message.splitlines():
        if line.find('Sender: ') >= 0:
            username = line[8:]
            return username
    return

def createStreamUsersFileName(streamname):
    return ('messages/' + streamname + 'StreamUsers')

def createStreamMessagesFileName(streamname):
    return ('messages/' + streamname + 'Stream')

def createStreamDataFileName(streamname):
    return ('messages/' + streamname + 'StreamData')

def main():
    if len(argv) < 2:
        print("No username provided.")
        return

    username = getUsername(argv[1:])

    viewNewStreams = True
    while viewNewStreams:
        streamnames = getStreamsWithUsername(username)
        if streamnames is None or streamnames == []:
            print("The user does not have access to any streams.")
            return
        streamsToView = getStreamsToView(streamnames)
        if streamsToView is None:
            print("The selected stream is not available.")
            return
        streamsPrevPostRead = getstreamsPrevPostRead(streamsToView, username)
        if streamsToView is None:
            print("Unable to obtain the last read post.")
            return
        viewNewStreams = viewStreams(streamsPrevPostRead, username)

if __name__ == '__main__':
    main()



