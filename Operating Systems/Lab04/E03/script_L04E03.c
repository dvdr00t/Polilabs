#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1000
#define MAX_ARGUMENTS_NUMBER 5

int main(int args, char *argv[]) {

    /*
    *   Checking the number of arguments passed by calling this progra;
    *   If the number is different by 2, send an error message.
    *   The arguments should be (1) the name of the program and (2) the file.
    */
    if (args != 2) {
       fprintf(stderr, "You should use this program by passing the name of a file!\n");
       exit(EXIT_FAILURE);
    }

    /* VARIABLES */
    FILE *fp_IN;
    char command[MAX_COMMAND_LENGTH];
    int switcher;

    /* Opening the file and checking */
    fp_IN = fopen(argv[1], "r");
    if (fp_IN == NULL) {
        fprintf(stderr, "Some errors occurred while trying to open %s file.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Choosing system call */
    fprintf(stdout, "Which system call do you want to use:\n1 - system()\n2 - exec()\n0 - exit()\n-->");
    fscanf(stdin, "%d", &switcher);

    switch (switcher) {
    case 1: //system()

        /* Reading from file */
        while (fgets(command, MAX_COMMAND_LENGTH, fp_IN) != NULL) {
            command[strlen(command) - 4] = '\0';
            system(command);
            // fprintf(stdout, "%s\n", command);
        }
        break;

    case 2: //exec()

        /* Reading from file */
        while (fgets(command, MAX_COMMAND_LENGTH, fp_IN) != NULL) {

            //Removing the END terminator
            command[strlen(command) - 4] = '\0';

            //Showing to the user what is going to be executed
            fprintf(stdout, "%s\n", command);

            /* Retrieving part of the command */
            char *UNIX_command = strtok(command, " ");  //First word --> UNIX command
            char *token = strtok(NULL, " ");    //Result of the strtok() to be stored in the list of arguments
            char **arg_ALL; //List of arguments

            //Allocating memory
            arg_ALL = (char**) malloc(MAX_ARGUMENTS_NUMBER * sizeof(char*));
            if (arg_ALL == NULL) {
                fprintf(stderr, "Some errors occurred while trying to allocate space in memory.\n");
                exit(EXIT_FAILURE);
            }
            for (int j = 0; j < 100; j++) {
                arg_ALL[j] = (char*) malloc(MAX_COMMAND_LENGTH * sizeof(char));
                if (arg_ALL[j] == NULL) {
                    fprintf(stderr, "Some errors occurred while trying to allocate space in memory.\n");
                    exit(EXIT_FAILURE);
                }
            }

            //Extracting token from command
            int i = 0;
            for (i = 1; token != NULL; i++) {
                strcpy(arg_ALL[i], token);
                token = strtok(NULL, " ");
            }

            //Adding NULL end argument for execvp() function
            arg_ALL[i] = NULL;

            

            /* Forking the program. Parent wait for the child while the child execute the command with execvp() */
            if (fork() == 0) {
                /* CHILD */
                sleep(3);
                execvp(UNIX_command, arg_ALL);
            }

            /* PARENT */
            wait((int *) 0);
        }
        
        break;

    
    default:
        break;
    }  

    fclose(fp_IN);
    exit(EXIT_SUCCESS);
    
}