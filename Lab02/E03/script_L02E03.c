#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 500
#define MAX_LINE_LENGTH 1000

int recurvise_copy(char *path_SRC, char *path_DST);   //Recursive function used to visit the tree
int file_copy(char *path_IN, char *path_OUT, char *filename);
int main (int args, char *argv[]) {

    /*
    * Program is called receiving on the command line TWO paths (absolute or relative) of two directories root.
    * If no path is passed or a wrong number of arguments is passed, close the program.
    */
    if (args != 3) {
       fprintf(stderr, "Two and only two path should be provided.\n");
       exit(EXIT_FAILURE);
   }

    /*
    * Recursive copy of the direction in the target path
    */ 
   fprintf(stdout, "\n----- Copying content of %s in %s:\n", argv[1], argv[2]);
   if (recurvise_copy(argv[1], argv[2]) != 0) {
       exit(EXIT_FAILURE);
   }
   fprintf(stdout, "----- Copying completed successfully!\n\n");
   exit(EXIT_SUCCESS);
}

int recurvise_copy(char *path_SRC, char *path_DST) {

    /*
    * Variables used
    */
    DIR *dp_SRC;            //Directory from which copy
    DIR *dp_DST;            //Directory to which copy
    struct dirent *dirp;    //Handle the iterating over the directory 
    struct stat buf;        //Handle the information of the current directory
    char *path_SRC_TMP;     //Path of the current element in working process in the directory
    char *path_DST_TMP;     //Path of the destination for the current element in working process in the directory


    /*
    * Opening the target directories.  
    * Checking that the openings have been successfully completed.
    */
    dp_SRC = opendir(path_SRC);
    dp_DST = opendir(path_DST);
    if (dp_SRC == NULL) {
       fprintf(stderr, "Opening the directory: %s failed during execution.\n", path_SRC);
       return 1;
    }
    if (dp_DST == NULL) {
       fprintf(stderr, "Opening the directory: %s failed during execution.\n", path_DST);
       return 1;
    }
    
    /*
    * Iterating over the whole source directory.
    * Every element is evaluated in order to verify if it is a FILE or a DIRECTORY
    * If it is a FILE: it is copied in the DST directory with a similar function from L01E04
    * If it is a DIRECTORY: it is created a new directory in the DST directory with the built-in function MKDIR 
    */ 
    while ((dirp = readdir(dp_SRC)) != NULL) {
      
        /*
        * Allocating space memory for storing the new target path.
        * Checking that the allocation has been succesfully completed and then storing the argument 
        * passed by the user.
        */
        path_SRC_TMP = (char*) malloc(sizeof(path_SRC) + sizeof(dirp->d_name) + 1);
        if (path_SRC_TMP == NULL) {
            fprintf(stderr, "Allocating memory space failed during execution.\n");
            return 1;
        }
        sprintf(path_SRC_TMP, "%s/%s", path_SRC, dirp->d_name);

        /* Acquiring information about the current path */
        if (lstat(path_SRC_TMP, &buf) < 0) {
            fprintf(stderr, "Reading the content of %s failed during execution.\n", path_SRC_TMP);
            return 1;
        }

        /*
        * Checking type of the current path and printing the results.
        */ 

        //FILE
        if (S_ISDIR(buf.st_mode) == 0) {
            file_copy(path_SRC_TMP, path_DST, dirp->d_name);
        }
        //DIRECTORY
        else if (S_ISREG(buf.st_mode) == 0) {

            /*
            * Verifying the the current element is not a link to the current directory or the parent directory
            */
            if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {

                /*
                * Allocating space memory for storing the new target path.
                * Checking that the allocation has been succesfully completed and then storing the argument 
                * passed by the user.
                */
                path_DST_TMP = (char*) malloc(sizeof(path_DST) + sizeof(dirp->d_name) + 1);
                if (path_DST_TMP == NULL) {
                    fprintf(stderr, "Allocating memory space failed during execution.\n");
                    return 1;
                }
                sprintf(path_DST_TMP, "%s/%s", path_DST, dirp->d_name);

                //Creating the new folder in the new path position
                mkdir(path_DST_TMP, buf.st_mode);

                //Calling recursive copy
                recurvise_copy(path_SRC_TMP, path_DST_TMP);

                /* Freeing memory */
                free(path_DST_TMP);
                free(path_SRC_TMP);
            }
        }
    }

    /*
    * Closing the directory.
    */
    if (closedir(dp_SRC) < 0) {
        fprintf(stderr, "Closing the directory: %s failed during execution.\n", path_SRC);
        return 1;
    }
    if (closedir(dp_DST) < 0) {
        fprintf(stderr, "Closing the directory: %s failed during execution.\n", path_DST);
        return 1;
    }

    return 0;
}
int file_copy(char *path_IN, char *path_OUT, char *filename) {

     /* Data variables */
    FILE *file_in;
    FILE *file_out;
    char *path_complete_OUT;
    char buffer[MAX_LINE_LENGTH];

    /*
    * Allocating space memory for storing the new target path.
    * Checking that the allocation has been succesfully completed and then storing the argument 
    * passed by the user.
    */
    path_complete_OUT = (char*) malloc(sizeof(path_OUT) + sizeof(filename) + 1);
    if (path_complete_OUT == NULL) {
        fprintf(stderr, "Some errors occurred while trying to open %s file line 157.\n", path_OUT);
        return 1;
    }
    sprintf(path_complete_OUT, "%s/%s", path_OUT, filename);

    /*
    * Opening target files.
    * Checking that the openings have been successfully completed.
    */ 
    file_in = fopen(path_IN, "r");
    file_out = fopen(path_complete_OUT, "w");
    if (file_in == NULL) {
        fprintf(stderr, "Some errors occurred while trying to open %s filef line 165.\n", path_IN);
        return 1;
    }
    if (file_out == NULL) {
        fprintf(stderr, "Some errors occurred while trying to open %s file line 169ip.\n", path_complete_OUT);
        return 1;
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

    /* Freeing memory */
    free(path_complete_OUT);

    return 0;
}
