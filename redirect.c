#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>


int redirect(char *command, char *restOfLine, char *path, int fromPara)   //checks for a redirect symbol, if found execution done here
{
    int redirectionFound = 0;
    char *string = (char*)malloc(1000);
    char *beforeRdrct = (char*)malloc(1000);                             //parsing a line
    char *afterRdrct = (char*)malloc(1000);  

    if(fromPara == 1)
    {
        string = strdup(command);
        string = strcat(string, " ");
        string = strcat(string,restOfLine);
    }
    else
        string = strdup(restOfLine);

    char redirect = '>';
    for(int i=0; i<=strlen(string); i++)                    //counting ">"
        if(string[i]==redirect)
            redirectionFound = redirectionFound + 1;

    if(redirectionFound == 1)                           //number of redirection symbols is valid, get everything after the ">"
    {
        beforeRdrct = strsep(&string,">");                   //everything before >
        afterRdrct = strsep(&string,"\n");                    //everything after >

    static char str1[99];
    int count = 0, j, k;
  
    while (afterRdrct[count] == ' ')
        count++;
  
    for (j = count, k = 0; afterRdrct[j] != '\0'; j++, k++)
        str1[k] = afterRdrct[j];

    str1[k] = '\0';
  
    afterRdrct = strdup(str1);

        if(strcmp(beforeRdrct,"") == 0)                       //nothing before >
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else if(strcmp(afterRdrct,"") == 0)                       //nothing after >
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else        //args exist after the ">"
        {
            char *found = (char*)malloc(1000);                             //parsing a line
            char *tempAfter = (char*)malloc(1000);
            tempAfter = strdup(afterRdrct);
            int argsAfter = 0;                                //counting arguments per line
            while( (found = strsep(&tempAfter," ")) != NULL )      //found contains a singular command
                if(strcmp(found,"") != 0)
                    argsAfter = argsAfter + 1;

            if(argsAfter == 1)                                      //one argument after redirect, fork, execv, and redirect in this
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
                while( (found = strsep(&string," ")) != NULL && (executed != 0) )      //found contains a singular path
                {
                    if(strcmp(found,"")!=0)                         //path is not some weird character
                    {
                        tempPath = strdup(found);
                        char *pch = strstr(tempPath, "\n");         //remove one newLine character from path
                        if(pch != NULL)
                            strncpy(pch, "\0", 1);
                        strcat(tempPath,"/");                       //add command to end of each path
                        strcat(tempPath,command);
 
                        char *myargs[3];
                        myargs[0] = strdup(command);        // program: "/bin/wc" (word count)
 
                        char cwd[256];
                        if (getcwd(cwd, sizeof(cwd)) == NULL)
                            perror("getcwd() error");
                        else
                        {
                            if(strcmp(beforeRdrct,"") == 0)
                                myargs[1] = strdup(cwd);      // arguments with command
                            else
                                myargs[1] = strdup(beforeRdrct);
                        }
 
                        char *noNewline = strstr(tempPath, "\n");         //remove one newLine character from path
                        if(noNewline != NULL)
                            strncpy(noNewline, "\0", 1);
 
                        char *remov = strstr(myargs[0], "\n");         //remove one newLine character from
                        if(remov != NULL)
                            strncpy(remov, "\0", 1);
 
                        char *noo = strstr(myargs[1], "\n");         //remove one newLine character from path
                        if(noo != NULL)
                            strncpy(noo, "\0", 1);
 
                        myargs[2] = NULL;                    // marks end of array
 
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
                                close(STDOUT_FILENO);
                                open(afterRdrct, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
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
            else                                                    //invalid amount of args after >
            {
                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }

        return redirectionFound;
    }
    else if(redirectionFound == 0)                                      //no redirection found, return 0 and execute normally
        return redirectionFound;
    else                                                                //invalid number of redirection, throw erre
    {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
        return redirectionFound;
    }
}