#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 50
#define MAX_LINE_LENGTH 1000

int main(int args, char *argv[]) {

    /* Checking the arguments */
    if (args != 3) {
        fprintf(stderr, "You should specifies three parameters: nameExcutable sourceFile destinationFile.\n");
        exit(EXIT_FAILURE);
    }

    /* Data variables */
    FILE *file_in;
    FILE *file_out;
    char filename_in[MAX_FILENAME_LENGTH];
    char filename_out[MAX_FILENAME_LENGTH];
    char buffer[MAX_LINE_LENGTH];

    /* Opening files and checking */
    strcpy(filename_in, argv[1]);
    strcpy(filename_out, argv[2]);

    file_in = fopen(filename_in, "r");
    if (file_in == NULL) {
        fprintf(stderr, "Some errors occurred while trying to open %s file.\n", filename_in);
        exit(EXIT_FAILURE);
    }
    file_out = fopen(filename_out, "w");
    if (file_out == NULL) {
        fprintf(stderr, "Some errors occurred while trying to open %s file.\n", filename_out);
        exit(EXIT_FAILURE);
    }

    /* Copying contents of the file_in into the file_out */
    while (fgets(buffer, sizeof(buffer), file_in) != NULL) {

        //Adding new line character
        char *newstr = malloc(strlen(buffer) + 2);
        strcpy(newstr, buffer);
        strcat(newstr, "\n");
        fputs(buffer, file_out);
    }

    /* Closing file */
    fclose(file_in);
    fclose(file_out);

    exit(EXIT_SUCCESS);
}