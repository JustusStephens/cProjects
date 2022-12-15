//finds out which command to execute, checks if valid
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     //for writing error message
#include "executeCommand.h"

void checkCommand(char *command, char *restOfLine, char *path)        //checking to see what command we have, command is the actual command, restOfLine is everything after
{
    char exitString[4] = "exit";            //defining strings for built in commands
    char cdString[2] = "cd";
    char pathString[4] = "path";
    int exitCompare = 1;
    int cdCompare = 1;
    int pathCompare = 1;
    pathCompare = strncmp(command,pathString,4);
    exitCompare = strncmp(command,exitString,4);     //compare first 4 chars of buffer to "exit"
    cdCompare = strncmp(command,cdString,2);

    if(exitCompare==0)                               //if equal then enter the built-in command
    {
        char *string = (char*)malloc(1000);
        char *found = (char*)malloc(1000);                             //parsing a line
        string = strdup(restOfLine);
        int argsPerLine = 0;                                //counting arguments per line
        while( (found = strsep(&string," ")) != NULL )      //found contains a singular command
            if(strcmp(found,"") != 0)
                argsPerLine = argsPerLine + 1;

        if(argsPerLine == 0)                          //valid exit, no args passed
        {
            exit(0);
        }
        else                                            //arguments were attempted to be passed to exit, throw error
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message)); 
        }
    }
    else if(cdCompare==0)                               //built-in cd
    {
        char *string = (char*)malloc(1000);
        char *found = (char*)malloc(1000);                             //parsing a line
        string = strdup(restOfLine);
        int argsPerLine = 0;                                //counting arguments per line
        while( (found = strsep(&string," ")) != NULL )      //found contains a singular command
            if(strcmp(found,"") != 0)
                argsPerLine = argsPerLine + 1;

        if(argsPerLine != 1)                             //cd given less or more than 1 argument, throw error
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message)); 
        }
        else                                                //else cd is valid, do command think need fork
        {
            //pass your path in the function
            char *pch = strstr(restOfLine, "\n");         //remove one newLine character from path
            if(pch != NULL)
                strncpy(pch, "\0", 1);
            int ch = chdir(restOfLine);
            if(ch<0)
                printf("chdir change of directory not successful");
            else
            {
                char cwd[256];
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    perror("getcwd() error");
            }
        }
    }
    else if(pathCompare==0)                              //built-in path
    {
        strcpy(path, restOfLine);
    }
    else                                            //command not built in
    {
        executeCommand(command, restOfLine, path, 0);
    }
}