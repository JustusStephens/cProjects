#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     //for writing error message
#include <ctype.h>

#include "checkCommand.h"

void batchMode(int argc, char *argv[], char *path)
{
    FILE * fp;
    char * line = (char*)malloc(1000);
    line = NULL;
    size_t len = 0;
    ssize_t read;
    if( access( argv[1], F_OK ) == 0 )                      //check access for file, (test 14)
    {
        const char *fileToOpen = (char*)malloc(1000);
        fileToOpen = argv[1];

        fp = fopen(fileToOpen, "r");

        int c = fgetc(fp);
        if (c == EOF)                                                       //throws error if passed empty batch file (test 13)
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message)); 
            exit(1);
        } 
        else 
        {
            ungetc(c, fp);
        }
        if (fp == NULL)
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message)); 
            exit(1);
        }
        

        while ((read = getline(&line, &len, fp)) != -1)     //line is the string
        {
            char *string = (char*)malloc(1000);
            char *found = (char*)malloc(1000);                            //parsing a line
            string = strdup(line);

            int i, x;                                                                   //removing whitspace
            for(i=x=0; string[i]; ++i)
                if(!isspace(string[i]) || (i > 0 && !isspace(string[i-1])))
                    string[x++] = string[i];
            string[x] = '\0';
            string[x-1] = '\0';                                                         //remove last whitespace

            int argsPerLine = 0;                                //counting arguments per line

            if(strcmp(string,"")!=0)
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
        }

        fclose(fp);
        if (line)
            free(line);
        exit(0);
    } 
    else 
    {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        exit(1);                                                            // file doesn't exist
    }
}