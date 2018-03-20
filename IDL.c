/*
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 3
    March 17, 2017
    This program allows for generating php files containing html and css from 
    configuration files with a .wpml suffix containing Interface Description Language.
***********************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IDL.h"

int main(int argc, char const* argv[])
{
    if (!hasIDLFileExtension(argv[1])) {
        printf("The IDL file must have a .wpml suffix.\n");
        return 1;
    }

/*    InterfaceElement** interfaceElements = getInterfaceElements(argv[1]);
    if (interfaceElements == NULL) {
        printf("The IDL file format is invalid.\n");
        return 1;
    }*/

    int createInterfaceFailure = createInterface(argv[1]);
    if (createInterfaceFailure) {
        printf("The IDL file format is invalid.\n");
        return 1;
    }
    return 0;
}

int hasIDLFileExtension(const char* fileName) {
    if (fileName == NULL) {
        return 0;
    }

    int fileNameLen = strlen(fileName);
    if (fileNameLen < 5) {
        return 0;
    }

    if (fileName[fileNameLen - 5] == '.' && fileName[fileNameLen - 4] == 'w' 
        && fileName[fileNameLen - 3] == 'p' && fileName[fileNameLen - 2] == 'm' 
        && fileName[fileNameLen - 1] == 'l') {
        return 1;
    }
    else {
        return 0;
    }
}

int createInterface(const char* IDLFileName) {
    FILE* IDLFile = fopen(IDLFileName, "r");
    if (IDLFile == NULL) {
        return 1;
    }

    printf("<html>\n"
        "<body>\n");

    char lineBuffer[1002];
    while (fgets(lineBuffer, 1002, IDLFile) != NULL) {
        /*line in file longer than buffer*/
        int lineBufferLen = strlen(lineBuffer);
        if (lineBuffer[lineBufferLen - 1] != '\n' && lineBuffer[lineBufferLen - 1] != ')') {
            return 1;
        }

        /*get the element tag character and the correpsonding attributes*/
/*printf("LINE:%s\n", lineBuffer);*/
        char* currentChar = &(lineBuffer[0]);
        while (*currentChar != '\r' && *currentChar != '\n' && *currentChar != '\0') {
            char elementTag = '\0';
            char tempAttributes[1002 - 4] = "";
            char attributes[1002 - 4] = "";
            
            int numMatches;
            numMatches = sscanf(currentChar, ".%c(%[^\n]", &elementTag, tempAttributes);
            if (numMatches < 2) {
                break;
            }

            /*attribute consists of string between '(', ')', and terminates with a '.'*/
            currentChar += 4;
            char* prevChar = currentChar - 1;
            int i = 0;
            while ((*currentChar != '.' || *prevChar != ')') && *currentChar != '\r' && *currentChar != '\n' && *currentChar != '\0') {
                attributes[i++] = *prevChar;
                attributes[i] = '\0';
/*printf("%c", *currentChar);*/
                currentChar++;
                prevChar++;
            }
/*printf("Att:%s\n\n", attributes);*/
            createInterfaceElement(elementTag, attributes);
        }
    }

    printf("</html>\n"
        "</body>\n");
    return 0;
}

int createInterfaceElement(char elementTag, char* attributes) {
    switch (elementTag) {
        case 'b':
            addButton(attributes);
            break;
        case 'd':
            addHorizontalLine(attributes);
            break;
        case 'e':
            addExecutable(attributes);
            break;
        case 'h':
            addHeading(attributes);
            break;
        case 'i':
            addInputForm(attributes);
            break;
        case 'l':
            addLink(attributes);
            break;
        case 'p':
            addPicture(attributes);
            break;
        case 'r':
            addRadioButtons(attributes);
            break;
        case 't':
            addText(attributes);
            break;
        case 'v':
            addWebViewCall(attributes);
            break;
        case 'm':
            addPostCall(attributes);
            break;
        case 'a':
            addAddAuthorCall(attributes);
            break;
        case 'z':
            addTextArea(attributes);
            break;
        default:
            printf("Invalid interface element type.\n");
            return 1;
            break;
    }
    printf("\n");
    return 0;
}

int extractKV(char* attributeToken, char* key, char* value) {
    return sscanf(attributeToken, "%[^=]=\"%[^\"]\"", key, value);
}

int extractKeyNum(char* attributeToken, char* key, int* num) {
    return sscanf(attributeToken, "%[^=]=%d", key, num);
}

int extractKeySize(char* attributeToken, char* key, int* width, int* height) {
    return sscanf(attributeToken, "%[^=]=%dx%d", key, width, height);
}

char* addAction(char* attributes) {
    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("action", key) == 0) {
                printf("action=\"%s\" ", value);
                break;
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            printf("%s ", attributeToken);
        }
        attributeToken = strtok(NULL, ",");
    }
    return attributeToken;
}

void addButton(char* attributes) {
    char name[1002 - 4] = "submit";
    char link[1002 - 4] = "";
    char btnText[1002 - 4] = "Submit";

    printf("<form method=\"POST\" ");

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("name", key) == 0) {
                strcpy(name, value);
            }
            else if (strcmp("link", key) == 0) {
                strcpy(link, value);
            }
            else if (strcmp("btnText", key) == 0) {
                strcpy(btnText, value);
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            printf("%s ", attributeToken);
        }
        attributeToken = strtok(NULL, ",");
    }
    printf("action=\"%s\">\n"
        "<input type=\"submit\" name=\"%s\" value=\"%s\"><br>\n"
        "</form>\n", 
        link, name, btnText);
    return;
}

void addHorizontalLine(char* attributes) {
    printf("<HR>\n");
    return;
}

void addExecutable(char* attributes) {
    char exe[1002 - 4] = "";

    printf("<?php\n");
    
    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("exe", key) == 0) {
                strcpy(exe, value);
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            printf("%s ", attributeToken);
        }
        attributeToken = strtok(NULL, ",");
    }

    printf("\t$exePath = $exe = \'%s\';\n"
        "\tif (!file_exists($exe)) {\n"
        "\t$exePath = \"./bin/\".$exe;\n"
        "\t\tif (!file_exists($exePath)) {\n"
        "\t\t\t$exePath = \"/bin/\".$exe;\n"
        "\t\t}\n"
        "\t}\n\n"
        "\texec($exePath, $output, $status);\n"
        "\tif ($status) {\n"
        "\t\techo \"exec failed\";\n"
        "\t}\n"
        "\telse {\n"
        "\t\tforeach ($output as $line) {\n"
        "\t\t\techo \"$line<br>\";\n"
        "\t\t}\n"
        "\t}\n"
        "?>\n"
        , exe);
    return;
}

void addHeading(char* attributes) {
    int fontSize = 3;
    int foundFontSize = 0;
    char text[1002 - 4] = "HEADING";

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";
        int tempSize;

        if (extractKeyNum(attributeToken, key, &tempSize) == 2) {
            if (strcmp("size", key) == 0) {
                if (fontSize >= 1 && fontSize <= 6) {
                    fontSize = tempSize;
                    foundFontSize = 1;
                    printf("<H%d ", fontSize);
                    break;
                }
            }
        }
        else if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("text", key) == 0) {
                strcpy(text, value);
            }
        }

        attributeToken = strtok(NULL, ",");
    }

    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("text", key) == 0) {
                strcpy(text, value);
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            printf("%s ", attributeToken);
        }

        attributeToken = strtok(NULL, ",");
    }

    if(!foundFontSize) {
        printf("<H%d", fontSize);
    }
    printf(">%s</H%d>\n", text, fontSize);
    return;
}

void addInputForm(char* attributes) {
    char btnText[1002 - 4] = "Submit";
    char btnName[1002 - 4] = "submit";

    printf("<form method=\"POST\" ");
    char* attributeToken = addAction(attributes);
    printf(">\n");

    int randomTextAllowed = 0;
    attributeToken = strtok(NULL, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("text", key) == 0) {
                printf("<p>%s<input ", value);
                randomTextAllowed = 1;
            }
            else if (strcmp("name", key) == 0) {
                printf("name=\"%s\" ", value);
            }
            else if (strcmp("value", key) == 0) {
                printf("value=\"%s\"></p>\n", value);
                randomTextAllowed = 0;
            }
            else if (randomTextAllowed){
                printf("%s ", attributeToken);
            }
            else if (strcmp("btnText", key) == 0) {
                strcpy(btnText, value);
            }
            else if (strcmp("btnName", key) == 0) {
                strcpy(btnName, value);
            }
        }
        else if(randomTextAllowed){
            printf("%s ", attributeToken);
        }
        attributeToken = strtok(NULL, ",");
    }
    printf("<p><input type=\"submit\" name=\"%s\" value=\"%s\"></p>\n"
        "</form>\n"
        , btnName, btnText);
    return;
}

void addLink(char* attributes) {
    char text[1002 - 4] = "link";
    char link[1002 - 4] = "";

    printf("<a ");

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("text", key) == 0) {
                strcpy(text, value);
            }
            else if (strcmp("link", key) == 0) {
                strcpy(link, value);
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            printf("%s ", attributeToken);
        }
        attributeToken = strtok(NULL, ",");
    }
    printf("href=\"%s\">%s</a><br>\n", link, text);
    return;
}

void addPicture(char* attributes) {
    char image[1002 - 4] = "";
    int width = 100;
    int height = 100;

    printf("<img ");

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";
        int tempNum1;
        int tempNum2;

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("image", key) == 0) {
                strcpy(image, value);
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            if (extractKeySize(attributeToken, key, &tempNum1, &tempNum2) == 3) {
                if (strcmp("size", key) == 0) {
                    width = tempNum1;
                    height = tempNum2;
                }
                else {
                    printf("%s ", attributeToken);
                }
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        attributeToken = strtok(NULL, ",");
    }
    printf("src=\"%s\" style=\"width:%d;height:%d;\"><br>\n", image, width, height);
    return;
}

void addRadioButtons(char* attributes) {
    char name[1002 - 4] = "";
    char btnText[1002 - 4] = "Submit";
    char btnName[1002 - 4] = "submit";

    /*form action*/
    printf("<form method=\"POST\" ");
    char* attributeToken = addAction(attributes);
    printf(">\n");

    /*name*/
    attributeToken = strtok(NULL, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("name", key) == 0) {
                strcpy(name, value);
                break;
            }
            else if (strcmp("btnText", key) == 0) {
                strcpy(btnText, value);
            }
            else if (strcmp("btnName", key) == 0) {
                strcpy(btnName, value);
            }
            else {
                /*printf("%s ", attributeToken);*/
            }
        }
        else {
            /*printf("%s ", attributeToken);*/
        }
        attributeToken = strtok(NULL, ",");
    }

    /*values*/
    int firstValue = 1;
    attributeToken = strtok(NULL, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("value", key) == 0) {
                if (!firstValue) {
                    printf("></p>\n");
                }
                printf("<p><input type=\"radio\" name=\"%s\" value=\"%s\"", name, value);
                if (firstValue) {
                    printf("checked ");
                    firstValue = 0;
                }
            }
            else if (strcmp("btnText", key) == 0) {
                strcpy(btnText, value);
            }
            else if (strcmp("btnName", key) == 0) {
                strcpy(btnName, value);
            }
            else {
                if (!firstValue) {
                    printf("%s ", attributeToken);
                }
            }
        }
        else {
            if (!firstValue) {
                printf("%s ", attributeToken);
            }
        }
        attributeToken = strtok(NULL, ",");
    }
    printf("><p>\n"
        "<input type=\"submit\" name=\"submit\" value=\"%s\"></p>\n"
        "</form>\n"
        ,btnText);
    return;
}

void addText(char* attributes) {
    char file[1002 - 4] = "";
    char text[1002 - 4] = "Default text";
    int usingFile = 0, firstUnknownAttribute = 1;

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("text", key) == 0) {
                strcpy(text, value);
            }
            else if (strcmp("file", key) == 0) {
                strcpy(file, value);
                usingFile = 1;
            }
            else {
                if (firstUnknownAttribute){
                    printf("<p ");
                    firstUnknownAttribute = 0;
                }
                printf("%s ", attributeToken);
            }
        }
        else {
            if (firstUnknownAttribute){
                printf("<p ");
                firstUnknownAttribute = 0;
            }
            printf("%s ", attributeToken);
        }

        attributeToken = strtok(NULL, ",");
    }

    if (!firstUnknownAttribute){
        printf(">\n");
    }
    if (usingFile) {
        FILE* formattedTextFile = fopen(file, "r");
        if (formattedTextFile) {
            while (fgets(file, 1002 - 4, formattedTextFile) != NULL) {
                printf("%s", file);
            }
        }
        else {
            printf("%s", text);
        }
    }
    else {
        printf("%s", text);
    }

    if (!firstUnknownAttribute){
        printf("\n</p>\n");
    }
    return;
}

void addWebViewCall(char* attributes) {
    char function[1002 - 4] = "";
    char args[1002 - 4] = "";

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("function", key) == 0) {
                strcpy(function, value);
            }
            else if (strcmp("args", key) == 0) {
                strcpy(args, value);
            }
        }
        attributeToken = strtok(NULL, ",");
    }

    printf("$output = [];\n"
        "//run with py for windows, python3 for unix\n"
        "exec(\"python3 webview.py %s %s\", $output, $status);\n", function, args);
    return;
}

void addPostCall(char* attributes) {
    char function[1002 - 4] = "";
    char args[1002 - 4] = "";

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("function", key) == 0) {
                strcpy(function, value);
            }
            else if (strcmp("args", key) == 0) {
                strcpy(args, value);
            }
        }
        attributeToken = strtok(NULL, ",");
    }

    printf("$output = [];\n"
        "//run with py for windows, python3 for unix\n"
        "exec(\"echo \\\"$streamForPost\".PHP_EOL.\"$messageForPost\\\" | \\\"./post\\\" $username\", $output, $status);\n");
    return;
}

void addAddAuthorCall(char* attributes) {
    char remove[1002 - 4] = "false";

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("remove", key) == 0) {
                strcpy(remove, value);
            }
        }
        attributeToken = strtok(NULL, ",");
    }

    printf("//run with py for windows, python3 for unix\n"
        "exec(\"echo \\\"$streamList\".PHP_EOL.\"\\\" | \\\"./addauthor\\\" ");
    if (strcmp(remove, "true") == 0) {
        printf("-r ");
    }
    printf("$author\", $output, $status);\n");
    return;
}

void addTextArea(char* attributes) {
    printf("<textarea ");

    char* attributeToken = strtok(attributes, ",");
    while (attributeToken != NULL) {
        char key[1002 - 4] = "";
        char value[1002 - 4] = "";

        if (extractKV(attributeToken, key, value) >= 1) {
            if (strcmp("rows", key) == 0) {
                printf("rows=\"%s\" ", value);
            }
            else if (strcmp("cols", key) == 0) {
                printf("cols=\"%s\" ", value);
            }
            else {
                printf("%s ", attributeToken);
            }
        }
        else {
            printf("%s ", attributeToken);
        }
        attributeToken = strtok(NULL, ",");
    }
    printf("</textarea>\n");
    return;
}
