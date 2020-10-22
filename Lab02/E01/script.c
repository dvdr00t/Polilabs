#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int tree_visit (char *path);   //Recursive function used to visit the tree
int main (int args, char *argv[]) {

    /*
    * Program is called receiving on the command line the path (absolute or relative) of a directory tree.
    * If no path is passed or a wrong number of arguments is passed, close the program.
    */
    if (args != 2) {
       fprintf(stderr, "One and only one path should be provided.\n");
       exit(EXIT_FAILURE);
   }

    /*
    * Variables used
    */
    char *target_path;  //Target path of the folder passed as argument

    /*
    * Allocating space memory for storing the target path.
    * Checking that the allocation has been succesfully completed and then storing the argument 
    * passed by the user.
    */
    target_path = (char*) malloc(sizeof(argv[1]));
    if (target_path == NULL) {
        fprintf(stderr, "Allocating memory space failed during execution.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(target_path, argv[1]);

    /*
    * Visit the tree.
    */ 
    if (tree_visit(target_path) == 0) {
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

int tree_visit (char *path) {

    /*
    * Variables used
    */
    DIR *dp;            //Directory handle
    struct dirent *dirp;    //Handle the iterating over the directory 
    struct stat buf;    //Handle the information of the current directory
    char *new_path; 


    /*
    * Opening the target directory.  
    * Checking that the opening has been successfully completed.
    */
    dp = opendir(path);
    if (dp == NULL) {
       fprintf(stderr, "Opening the directory: %s failed during execution.\n", path);
       return 1;
    }
    fprintf(stdout, "----- Listing content of %s:\n", path);

    /*
    * Iterating over the whole directory.
    * First cycle is used only to print the content of the directory.
    * A second cycle will be used to provide the visit of the subdirectories.
    */ 
    while ((dirp = readdir(dp)) != NULL) {
      
        /*
        * Allocating space memory for storing the new target path.
        * Checking that the allocation has been succesfully completed and then storing the argument 
        * passed by the user.
        */
        new_path = (char*) malloc(sizeof(path) + sizeof(dirp->d_name) + 1);
        if (new_path == NULL) {
            fprintf(stderr, "Allocating memory space failed during execution.\n");
            return 1;
        }
        sprintf(new_path, "%s/%s", path, dirp->d_name);

        /* Acquiring information about the current path */
        if (lstat(new_path, &buf) < 0) {
            fprintf(stderr, "Reading the directory: %s failed during execution.\n", new_path);
            return 1;
        }

        /*
        * Checking type of the current path and printing the results.
        */ 
        if (S_ISDIR(buf.st_mode) == 0) {
            fprintf(stdout, "FILE: %s\n", dirp->d_name);
        }
        else if (S_ISREG(buf.st_mode) == 0) {
            fprintf(stdout, "DIRECTORY: %s\n", dirp->d_name);
        }
    }
    /* END OF THE FIRST CYCLE */


    if (closedir(dp) < 0) {
        fprintf(stderr, "Closing the directory: %s failed during execution.\n", path);
        return 1;
    }
    fprintf(stdout, "\n");
    dp = opendir(path);
    if (dp == NULL) {
        fprintf(stderr, "Opening the directory: %s failed during execution.\n", path);
        return 1;
    }
  
    /*
    * Iterating over the whole directory.
    * Second cycle is used to enter in the subdirectories.
    */ 
    while ((dirp = readdir(dp)) != NULL) {
      
        /*
        * Allocating space memory for storing the new target path.
        * Checking that the allocation has been succesfully completed and then storing the argument 
        * passed by the user.
        */
        new_path = (char*) malloc(sizeof(path) + sizeof(dirp->d_name) + 1);
        if (new_path == NULL) {
            fprintf(stderr, "Allocating memory space failed during execution.\n");
            return 1;
        }
        sprintf(new_path, "%s/%s", path, dirp->d_name);

        /* Acquiring information about the current */
        if (lstat(new_path, &buf) < 0) {
            fprintf(stderr, "Reading the directory: %s failed during execution.\n", new_path);
            return 1;
        }

        /*
        * Checking if the subdirectory is not a "." or a ".." directories in order to avoid infinite recursions.
        */ 
        if (S_ISREG(buf.st_mode) == 0) {
            if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
                tree_visit(new_path);
            } 
        }
    }
    /* END OF THE SECOND CYCLE */

    /*
    * Closing the directory.
    */
    if (closedir(dp) < 0) {
      fprintf(stderr, "Closing the directory: %s failed during execution.\n", path);
      return 1;
    }

    return 0;
}
