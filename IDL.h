/*
***********************************************************************************
    Daniel Kolodziejczak
    
    Assignment 3
    March 17, 2017
    This program allows for generating php files containing html and css from 
    configuration files with a .wpml suffix containing Interface Description Language.
***********************************************************************************
*/

/**
    For all interface elements:
    - lines in the IDL file cannot exceed 1000 characters excluding newline
    - each interface element is denoted by a period followed by a character representing the 
    interface element and a list of attributes that appear between brackets
    - there can be multiple interface elements per line, with no leading or trailing space, or 
    spaces between element ending bracket and next element period
    - attributes are a comma separated list of key value pairs and unknown attributes
    - attributes cannnot contain commas
    - attributes cannot contain spaces between any of comma and key, key and equals, 
    equals and value, value and comma
    - values in double quotes cannot contain double quotes
    - unknown attributes not contained in the attributes list of an interface element can be any 
    valid string not containing a comma and may be printed to specified tags
    - the order that attributes should appear in will be specifed by numbers, X meaning it can 
    appear anywhere, and attributes listed under the same number can appear anywhere
    - attribute types with defaults are optional to include in the attribute list
**/

#ifndef __IDL_H__
#define __IDL_H__

#include <stdlib.h>

int hasIDLFileExtension(const char* fileName);

int createInterface(const char* IDLFileName);

int createInterfaceElement(char elementTag, char* attributes);

int extractKV(char* attributeToken, char* key, char* value);

int extractKeyNum(char* attributeToken, char* key, int* num);

int extractKeySize(char* attributeToken, char* key, int* width, int* height);

/*
    1.
    action="<action>"
    - prints unknown attributes in place
*/
char* addAction(char* attributes);

/*
    b Button - Add a clickable button.
    Uses a form with the post method.

    1.
    name="<name>" -string containing an identifier for the button
    link="<link>" -string containing the page linked to the button
    btnText="<btnText>" -string containing text displayed on the button
        default: "Submit"
    - prints unknown attributes in opening form tag
*/
void addButton(char* attributes);

/*
    d Horizontal Line - Draw a horizontal line.
    - does not print any attributes
*/
void addHorizontalLine(char* attributes);

/*
    e Executable - Add code that runs a program on the server.
    Checks for file in the current directory, bin directory, and system bin directory
    in that order and will execute the first valid path found.

    1.
    exe="<filename>" -executes the program <filename> on the server
    - prints unknown attributes in opening php tag
*/
void addExecutable(char* attributes);

/*
    h Heading - Add a heading to the pagee.

    1.
    size=<1-6>
    text="<text>"
        default: "HEADING"
    - does not print unknown attributes preceding size
    - prints unknown attributes in opening tag
*/
void addHeading(char* attributes);

/*
    i Input - Allow the user to submit one or more text fields.
    Uses a form with the post method.

    1.
    action="<filename>" -indicates the page which processes the buttons
    - prints unknown attributes in opening form tag

    2.
    text="..." -the label which is printed before the field
    name="<name>" -name which the text field will be associated with
    value="<value>" -indicates the default value for a field
    - must appear as sets of 3 in any order any number of times
    - unknown attributes must appear after text but before the value attributes
    - prints unknown attributes to corresponding input field

    btnText="<btnText>" -string containing text displayed on the button
        default: "Submit"
    btnName="<btnName> -string containing the name of the button
        default: "submit"

*/
void addInputForm(char* attributes);

/*
    l Link - Link to a web page.

    1.
    text="<text>" -the text between the quotes is printed as the link
        default: "link"
    link="<url>" -contains the URL to which the link leads 
*/
void addLink(char* attributes);

/*
    p Picture - Insert an image into the page.

    1.
    image="<image>" -contains the image name
    size=<width>x<height> -indicates the width and height of the image
        default: 100x100
*/
void addPicture(char* attributes);

/*
    r Radio Button - Provide a list of options, the user can select one.
    Uses a form with the post method.

    1.
    action="<filename>" -indicates the page which processes the buttons.
    - prints unknown attributes in opening form tag preceding the action attribute

    2.
    name="<name>" -name which the result will be associated with
    - does not print any unknown attributes preceding or immediately following 
    the name attribute
    
    3.
    value="<value>" -indicates the value for a radio button
    - can appear any number of times, once for each radio button to create
    - the first value is checked by default
    - prints unknown attributes in tag of the last value attribute given

    2 or 3.
    btnText="<btnText>" -string containing text displayed on the button
        default: "Submit"
    btnName="<btnName> -string containing the name of the button
        default: "submit"

*/
void addRadioButtons(char* attributes);

/*
    t Text - Add text to the page.

    1.
    file="<filename>" -the text is in the file named <filename>
    text="<text>" -the text is the string between the quotes
        default: "Default text"

    - only one argument is used
    - if both attributes are given, the file takes precedence over text,
    if the file cannot be opened, the text will be used 

    - if no unknown attributes are used, plain text will be outputted.
    - if unknown attributes are used, the paragraph tag will be used with 
    all unknown attributes in the opening tag
*/
void addText(char* attributes);

/*
    v WebViewCall - add call to webview application

    1.
    function="<function>" -the function to call
    args=<"args"> -the arguments to the funciton call
    - does not print any unknown attributes
*/
void addWebViewCall(char* attributes);

/*
    m Post Call - add call to post application
    - does not print any attributes
    
*/
void addPostCall(char* attributes);

/*
    a Add Author Call - add call to add author application

    1.
    remove="true" -specify that the program attempt to remove the author from a stream
        default: "false"
    - does not print any unknown attributes
*/
void addAddAuthorCall(char* attributes);

void addTextArea(char* attributes);

#endif
