#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>


#define ONLY_READ "r"
#define ONLY_WRITE "w"
#define MAX_LINE_LENGTH 100

/*
*   + --------- +
*   | FUNCTIONS |
*   + --------- +
*/

/**
 * @brief Copy recursively the directory tree at the input source path to the input destination path.
 * 
 * @param src_path 
 * @param dst_path 
 * @return int 
 */
int copy_directory(char *src_path, char *dst_path);

/**
 * @brief Copy the file at the input source path to the input destination path.
 * 
 * @param src_path 
 * @param dst_path 
 * @param filename 
 * @return int 
 */
int file_copy(char *src_path, char *dst_path, char *filename);

/**
 * @brief Main driver code.
 * 
 * @param args 
 * @param argv 
 * @return int 
 */
int main (int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args != 3) {
        fprintf(stderr, "[USAGE] ./exe03 path_1 path_2\n\n");
        exit(EXIT_FAILURE);
    }

    /* PARSING USER INPUT */
    char *path_1 = argv[1];
    char *path_2 = argv[2];

    /* CHECKING USER INPUT IS AN EXISTING DIRECTORY */
    struct stat buffer;
    if (stat(path_1, &buffer) != 0) {
        fprintf(stderr, "[ERROR] stat() function failed execution\n\n");
        exit(EXIT_FAILURE);
    }
    if (!S_ISDIR(buffer.st_mode)) {
        fprintf(stderr, "[ERROR] %s is not a path to an existing direectory...\n\n", path_1);
        exit(EXIT_FAILURE);
    } 

    /* INITIATING COPY PROCESS */
    if (copy_directory(path_1, path_2) != 0) {
        fprintf(stderr, "[ERROR] copying directory tree failed execution\n\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

int copy_directory(char *src_path, char *dst_path) {

    char *src_path_TMP;     //Path of the current element in working process in the directory
    char *dst_path_TMP;     //Path of the destination for the current element in working process in the directory

    /* OPENING SOURCE DIRECTORY AND CHECKING ERRORS */
    DIR *dp_src = opendir(src_path);
    if (dp_src == NULL) {
        fprintf(stderr, "[ERROR] opendir() function failed execution\n\n");
        return -1;
    }

    /* CREATING AND OPENING DESTINATION DIRECTORY AND CHECK FOR ERRORS */
    struct stat buffer;
    DIR *dp_dst;
    if (!(stat(dst_path, &buffer) == 0 && S_ISDIR(buffer.st_mode))) {

        /* DESTINATION DIRECTORY DOES NOT EXIST */
        if (mkdir(dst_path, S_IRWXO | S_IRWXU | S_IRWXG) != 0) {
            fprintf(stderr, "[ERROR] mkdir() function failed execution\n\n");
            return -1;
        }
    }
    dp_dst = opendir(dst_path);
    if (dp_dst == NULL) {
        fprintf(stderr, "[ERROR] opendir() function failed execution\n\n");
        return -1;
    }

    /* VISIT RECURSIVELY ALL THE TREE */
    struct dirent *dirp;
    while ((dirp = readdir(dp_src)) != NULL) {

        /*
        * ALLOCATING SPACE IN MEMORY FOR STORING TARGET PATH.
        * ---------------------------------------------------
        * Checking that the allocation has been successfully completed and then storing the argument 
        * passed by the user.
        */
        src_path_TMP = (char*) malloc((strlen(src_path) + strlen(dirp->d_name) + 1) * sizeof(char));
        if (src_path_TMP == NULL) {
            fprintf(stderr, "[ERROR] malloc() function failed execution\n\n");
            return -1;
        }
        sprintf(src_path_TMP, "%s/%s", src_path, dirp->d_name);

        /* CHECKING IF CURRENT VISITED NODE IS A FILE OR A DIRECTORY */
        if (lstat(src_path_TMP, &buffer) < 0) {
            fprintf(stderr, "[ERROR] Reading the content of %s failed during execution\n\n", src_path_TMP);
            return -1;
        }
        if (!S_ISDIR(buffer.st_mode)) {

            /* COPYING FILE */
            fprintf(stdout, "> Copying %s ...\n", src_path_TMP);
            if (file_copy(src_path_TMP, dst_path, dirp->d_name) != 0) {
                fprintf(stderr, "[ERROR] file_copy() failed execution\n\n");
                return -1;
            }
        } else {
            if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {

                /*
                * ALLOCATING SPACE IN MEMORY FOR STORING TARGET PATH.
                * ---------------------------------------------------
                * Checking that the allocation has been successfully completed and then storing the argument 
                * passed by the user.
                */
                dst_path_TMP = (char*) malloc((strlen(dst_path) + strlen(dirp->d_name) + 1) * sizeof(char));
                if (dst_path_TMP == NULL) {
                    fprintf(stderr, "Allocating memory space failed during execution.\n");
                    return 1;
                }
                sprintf(dst_path_TMP, "%s/%s", dst_path, dirp->d_name);

                /* COPY DIRECTORY RECURSIVELY */
                fprintf(stdout, "> Copying %s ...\n", src_path_TMP);
                if (copy_directory(src_path_TMP, dst_path_TMP) != 0) {
                    fprintf(stderr, "[ERROR] copying directory tree failed execution\n\n");
                    return -1;
                }
                /* FREEING MEMORY */
                free(dst_path_TMP);
            }
        }
        /* FREEING MEMORY */
        free(src_path_TMP);
    }

    /* CLOSING SOURCE DIRECTORY AND CHECKING ERRORS */
    if (closedir(dp_dst) != 0 || closedir(dp_src) != 0) {
        fprintf(stderr, "[ERROR] closedir() function failed execution\n\n");
        return -1;
    }

    return 0;
}

int file_copy(char *src_path, char *dst_path, char *filename) {

    /*
    * ALLOCATING SPACE IN MEMORY FOR STORING TARGET PATH.
    * ---------------------------------------------------
    * Checking that the allocation has been successfully completed and then storing the argument 
    * passed by the user.
    */
    char *dst_path_NEW = (char*) malloc((strlen(dst_path) + strlen(filename) + 1) * sizeof(char));
    if (dst_path_NEW == NULL) {
        fprintf(stderr, "Allocating memory space failed during execution.\n");
        return 1;
    }
    sprintf(dst_path_NEW, "%s/%s", dst_path, filename);

    /* OPENING FILES */
    FILE *fp_src = fopen(src_path, ONLY_READ);
    FILE *fp_dst = fopen(dst_path_NEW, ONLY_WRITE);
    if (fp_src == NULL || fp_dst == NULL) {
        fprintf(stderr, "[ERROR] file opening failed execution\n\n");
        return -1;
    }

    /* COMPUTING SIZE OF SOURCE FILE */
    fseek(fp_src, 0L, SEEK_END);
    int size = ftell(fp_src);

    /* ADDING HEADER TO THE DESTINATION FILE */
    fprintf(fp_dst, "[HEADER] %s\n", filename);
    fprintf(fp_dst, "[HEADER] %d\n\n", size);
    rewind(fp_src);

    /* COPYING FILE CONTENT */
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, fp_src) != NULL) {
        fprintf(fp_dst, "%s\n", buffer);
    }

    /* CLOSING FILES AND FREEING MEMORY */
    fclose(fp_src);
    fclose(fp_dst);
    free(dst_path_NEW);

    return 0;
}