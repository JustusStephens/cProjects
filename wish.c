#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     //for writing error message
#include "interactiveMode.h"
#include "batchMode.h"

int main(int argc, char *argv[])                            //argv[0] is name of executable
{
    char *path = (char*)malloc(1000);                             //parsing a line
    path = strdup("/bin");

    if(argc == 1)               //one argument, enter interactive
        interactiveMode(argc, argv, path);
    else if (argc > 1)
        batchMode(argc, argv, path);            //multiple, enter batch mode
    else
    {
        printf("Error, couldnt enter batch or interactive");
        exit(0);
    }
}