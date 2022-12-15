#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "redirect.h"
#include "parallel.h"

void executeCommand(char *command, char *restOfLine, char *path, int fromPara)
{
    int paralleled = parallel(command,restOfLine,path);
    if(paralleled == 0)                                                    //no parallel, continue normally
    {
        int redirected = redirect(command,restOfLine,path, fromPara);
        if(redirected == 0)                                                //no redirection continue normally
        {
            char *string = (char*)malloc(1000);
            char *found = (char*)malloc(1000);                             //parsing a line
            char *tempPath = (char*)malloc(1000);
            string = strdup(path);
            int executed = 5;
            if(strcmp(path,"") == 0)
            {
                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
            else
            while( (found = strsep(&string," ")) != NULL && (executed != 0) )      //loop that attempts to execute paths
            {
                if(strcmp(found,"")!=0)                         //path is not some weird character
                {
                    tempPath = strdup(found);
                    char *pch = strstr(tempPath, "\n");         //remove one newLine character from path
                    if(pch != NULL)
                        strncpy(pch, "\0", 1);
                    strcat(tempPath,"/");                       //add command to end of each path
                    strcat(tempPath,command);

                    char cwd[256];
                    if (getcwd(cwd, sizeof(cwd)) == NULL)
                        perror("getcwd() error");
                    else
                    {
                        if(strcmp(restOfLine,"") == 0)          //this is for commands that needs cd path
                        {
                            char *myargs[3];
                            myargs[0] = strdup(command);        // program: "/bin/wc" (word count)
                            myargs[1] = strdup(cwd);            // arguments with command
                            myargs[2] = NULL;

                            char *remov = strstr(myargs[0], "\n");         //remove one newLine character from 
                            if(remov != NULL)
                                strncpy(remov, "\0", 1);

                            char *noo = strstr(myargs[1], "\n");             //remove one newLine character from path
                            if(noo != NULL)
                                strncpy(noo, "\0", 1);

                            char *noNewline = strstr(tempPath, "\n");         //remove one newLine character from path
                            if(noNewline != NULL)
                                strncpy(noNewline, "\0", 1);

                            int fileExist = access(tempPath, X_OK); //returns -1 if no access
                            executed = fileExist;

                            if(fileExist != -1)
                            {
                                int rc = fork();

                                if (rc < 0)         //fork failed
                                {
                                    fprintf(stderr, "fork failed\n");
                                    exit(1);
                                } 
                                
                                else if (rc == 0)   //child, do execv in this, arg[0] needs concat with each path /bin/ls... /usr/bin/ls
                                {
                                    execv(tempPath, myargs);
                                } 
                                else                //parent waits for child to finish
                                {
                                    int wc = wait(NULL);
                                    assert(wc >= 0);
                                }
                            }

                            else if(string != NULL)     //still paths left to try, dont throw error yet
                            {
                    
                            }

                            else                                    //access failed, throw error and kill child
                            {
                                char error_message[30] = "An error has occurred\n";
                                write(STDERR_FILENO, error_message, strlen(error_message));
                            }
                        }
                        else                                                //read in x amount of args
                        {
                            char *found = (char*)malloc(1000);                             //parsing a line
                            char *tempRestOfLine = (char*)malloc(1000);
                            tempRestOfLine = strdup(restOfLine);
                            int argNum = 0;                                //counting arguments per line
                            while( (found = strsep(&tempRestOfLine," ")) != NULL )      //found contains a singular command
                                if(strcmp(found,"") != 0)
                                    argNum = argNum + 1;
                            char *myargs[argNum+2];             //either argNum or argNum +1 or +2
                            myargs[0] = strdup(command);        // program: "/bin/wc" (word count)
                            int num = 1;
                            while( (found = strsep(&restOfLine," ")) != NULL && (executed != 0) )
                            {
                                myargs[num] = strdup(found);
                                char *noNewline = strstr(myargs[num], "\n");         //remove one newLine character from path
                                if(noNewline != NULL)
                                    strncpy(noNewline, "\0", 1);
                                num = num + 1;
                            } 
                            myargs[num] = NULL;

                            char *noNewline = strstr(tempPath, "\n");         //remove one newLine character from path
                            if(noNewline != NULL)
                                strncpy(noNewline, "\0", 1);

                            int fileExist = access(tempPath, X_OK); //returns -1 if no access
                            executed = fileExist;

                            if(fileExist != -1)
                            {
                                int rc = fork();

                                if (rc < 0)         //fork failed
                                {
                                    fprintf(stderr, "fork failed\n");
                                    exit(1);
                                } 
                                
                                else if (rc == 0)   //child, do execv in this, arg[0] needs concat with each path /bin/ls... /usr/bin/ls
                                {
                                    execv(tempPath, myargs);
                                } 
                                else                //parent waits for child to finish
                                {
                                    int wc = wait(NULL);
                                    assert(wc >= 0);
                                }
                            }

                            else if(string != NULL)     //still paths left to try, dont throw error yet
                            {
                    
                            }

                            else                                    //access failed, throw error and kill child
                            {
                                char error_message[30] = "An error has occurred\n";
                                write(STDERR_FILENO, error_message, strlen(error_message));
                            }
                        }
                    }
                }          
            }
        }   //redirect
    }   //parallel
}