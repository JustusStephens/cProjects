#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     //for writing error message
#include <ctype.h>
#include "checkCommand.h"

void interactiveMode(int argc, char **argv[], char *path)
{
    char *buffer;
    size_t bufsize = 32;
    size_t characters;
    char exitString[4] = "exit";            //defining strings for built in commands
    char cdString[2] = "cd";
    char pathString[4] = "path";
    int exitCompare = 1;
    int cdCompare = 1;
    int pathCompare = 1;
    while(exitCompare!=0)
    {
        buffer = (char *)malloc(bufsize * sizeof(char));
        if( buffer == NULL)
        {
            perror("Unable to allocate buffer");
            exit(1);
        }
        printf("\n%s", "wish> ");                          //printing info
        characters = getline(&buffer,&bufsize,stdin);

        char *string,*found;                            //parsing a line
        string = strdup(buffer);
        int argsPerLine = 0; 
        
        int i, x;                                                                   //removing whitspace
        for(i=x=0; string[i]; ++i)
            if(!isspace(string[i]) || (i > 0 && !isspace(string[i-1])))
                string[x++] = string[i];
        string[x] = '\0';
        string[x-1] = '\0';                                                         //remove last whitespace

        if(strcmp(string,"")!=0)                               //counting arguments per line
        while( (found = strsep(&string," ")) != NULL )      //found contains a singular command
        {
            if(argsPerLine == 0)        //first command of the line
            {
                if(string == NULL)
                    checkCommand(found,&string,path);
                else
                    checkCommand(found,string,path);
            }
            argsPerLine = argsPerLine + 1;
        }
        char *buffer;                                     //reset buffer so no seg fault
        size_t bufsize = 32;
        size_t characters;
    }
}