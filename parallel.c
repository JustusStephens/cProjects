#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include "executeCommand.h"
#include "checkCommand.h"
#include <ctype.h>

int spaceCount = 0;
int countCommand = 0;

int parallel(char *command, char *restOfLine, char *path)   //checks for a parallel symbol, if found execution done here, else return 0
{
    int paraFound = 0;
    char *string = (char*)malloc(1000);
    char *check16 = (char*)malloc(1000);                    //checks for a single &
    char parallel = '&';

    check16 = strdup(command);                              //if single &, and nothing else, just return
    for(int i=0; i<=strlen(check16); i++)   
    if(check16[i] == parallel)
        paraFound = paraFound + 1;

    if(paraFound == 1)
        return 1;

    string = strdup(restOfLine);
    for(int i=0; i<=strlen(string); i++)                    //counting ">"
    if(string[i]==parallel)
        paraFound = paraFound + 1;

    if(paraFound == 0)
        return 0;

    else                                                    //parallel exists with args
    {
        char *combine = (char*)malloc(1000);  
        if(spaceCount == 0)
        {
            combine = strcat(check16, " ");
            spaceCount = 1;
        }
        combine = strcat(combine, string);

        char *splitArray[paraFound+1];
        int x = 0;
        while((splitArray[x] = strsep(&combine,"&")) != NULL)
        {
            char *temp;                                             //remove spaces from commands
            temp=splitArray[x];  
            while (*temp==' ') temp++;  
            if (temp!=splitArray[x]) 
                memmove(splitArray[x],temp,strlen(temp)+1);

            if(strcmp(splitArray[x],"") != 0)                       //parallel command valid, now needs split into command and rest of line
            {
                int len = strlen(splitArray[x]);

                char *tempString = (char*)malloc(1000);  
                tempString = strdup(splitArray[x]);

                char *end;

                end = tempString + strlen(tempString) - 1;                              //remove trailing space
                while(end > tempString && isspace((unsigned char)*end)) end--;

                // Write new null terminator character
                end[1] = '\0';

                splitArray[x] = strdup(tempString);

                char *found = (char*)malloc(1000);
                char *string2 = (char*)malloc(1000);  
                string2 = strdup(splitArray[x]);  

                char *commandArray[paraFound+1];
                char *restOfLineArray[paraFound+1];

                int argsPerLine = 0; 

           if(strcmp(string2,"")!=0)
                while( (found = strsep(&string2," ")) != NULL )      //found contains a singular command
                {
                    if(argsPerLine == 0)        //first command of the line
                    {
                        argsPerLine = 2;

                        if(string2 == NULL)
                            executeCommand(found,&string2,path,0);
                        else
                        {
                            commandArray[countCommand] = strdup(found);
                            restOfLineArray[countCommand] = strdup(string2);

                            countCommand = countCommand + 1;

                            executeCommand(commandArray[countCommand-1],restOfLineArray[countCommand-1],path,1);
                        }
                    }
                }

            }

            x = x + 1;
        }

        return 1;
    }
}