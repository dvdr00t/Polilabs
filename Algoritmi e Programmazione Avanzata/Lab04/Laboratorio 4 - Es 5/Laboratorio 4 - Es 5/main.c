//
//  main.c
//  Laboratorio 4 - Es 5
//
//  Created by Davide Arcolini on 12/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME 15
#define MAX_LINE 1000
#define MAX_STRING 30

typedef struct{                         //Storing data from file
    char code[MAX_STRING];       //Identifier code
    char start[MAX_STRING];      //Leaving station
    char end[MAX_STRING];        //Arriving station
    char date[MAX_STRING];       //Date
    char time_start[MAX_STRING]; //Leaving time
    char time_end[MAX_STRING];   //Arriving time
    int delay;
} dato;

dato* merge_sort(dato *transport_A, dato *transport_B, int n, int operation);   //Main mergesort function
void merge_sort_r(dato *transport, int left, int right, int operation);         //Mergesort wrapper
void merge(dato *transport, int left, int medium, int right, int operation);    //Ricombination function
void linear_search(char *station_name, dato *transport, int rows);
void binary_search(char *station_name, dato *transport, int rows);
void my_toupper(char *string);                                         //Converting a string in uppercase (to compare strings always in uppercase)
void print_menu(void);                                                 //Showing the menu
void print_line(dato *transports, int i);                              //Showing a particular line of the data saved in the struct
void fprint_line(FILE *fp, dato *transports, int i);                   //Printing on a FILE a particular line of the data saved in the struct
dato* reading_file(FILE *fp, int *rows);                               //Reading data from file and saving it in a struct
void checking_file(FILE *fp, char *filename);                          //Checking correct opening for a file
int main(int argc, const char * argv[]){

    //Checking command line parameters
    if (argc != 2){
        fprintf(stderr, "ERROR: insert only program name and file name.\n\n");
        exit(EXIT_FAILURE);
    }
    char filename[MAX_FILENAME];
    strcpy(filename, argv[1]);
    char filename_print[MAX_FILENAME], station_name[MAX_STRING];
    int rows, i;;
    int flag = 0, command;  //Flag is used to keep the "while" loop going. Command switches between the operations
    char command_confirm;   //Used to confirm the termination operation
    
    //transports is the original array of "dato" in which are stored the information from file. transports_B is used to point the particular dato element in which we found the leaving station required by the user
    dato *transports = NULL;
    //transport_sorted store all the sorted data (0 for date_sorted, 1 for code_sorted, etc...)
    dato *transports_sorted[4] = {NULL};
    //fp is the log file. fp_print is the file in which the information are printed by the users
    FILE *fp = NULL, *fp_print = NULL;
    fp = fopen(filename, "r");  //Opening log file
    checking_file(fp, filename);    //Checking if the open worked well
    transports = reading_file(fp, &rows);   //Reading the file and store data
    fclose(fp); //Closing log file
    
    while (flag == 0){
        print_menu();   //Printing menu
        scanf("%d", &command);  //Reading command
        
        switch (command){
                //Showing/Printing data
            case 1:{
                printf("\n1) Show all data.\n");
                printf("2) Print all data in a file.\n");
                printf("3) Exit.\n");
                printf("\nSelect your operation: ");
                scanf("%d", &command);
                if (command == 1){
                    printf("\n\nShowing all data:\n");
                    for (i = 0; i < rows; i++)
                        print_line(transports, i);  //Showing data from original file
                    printf("\n\n");
                }
                else if (command == 2){
                    printf("Insert file name: ");
                    scanf("%s", filename_print);
                    fp_print = fopen(filename_print, "w");
                    printf("\n1) Print sorted data.\n");
                    printf("2) Print unsorted data.\n");
                    printf("3) Exit.\n");
                    printf("\nSelect your operation: ");
                    scanf("%d", &command);
                    if (command == 2)
                        for (i = 0; i < rows; i++)
                            fprint_line(fp_print, transports, i);   //Printing in a file original data (UNSORTED)
                    else if (command == 1){
                        printf("Choose key for sorting:\n");
                        printf("1) Sorted by dates.\n");
                        printf("2) Sorted by codes.\n");
                        printf("3) Sorted by leaving stations.\n");
                        printf("4) Sorted by arriving stations.\n");
                        printf("5) Exit.\n");
                        printf("\nSelect your key: ");
                        scanf("%d", &command);
                        
                        //Printing in a file original data (SORTED)
                        //Sorts will be stored for next operations (next sorts will be already done if we choose to print on a file SORTED data
                        switch (command){
                            case 1:
                                transports_sorted[0] = merge_sort(transports, transports_sorted[0], rows, 2);
                                for (i = 0; i < rows; i++)
                                    fprint_line(fp_print, transports_sorted[0], i);
                                break;
                            case 2:
                                transports_sorted[1] = merge_sort(transports, transports_sorted[0], rows, 3);
                                for (i = 0; i < rows; i++)
                                    fprint_line(fp_print, transports_sorted[1], i);
                                break;
                            case 3:
                                transports_sorted[2] = merge_sort(transports, transports_sorted[0], rows, 4);
                                for (i = 0; i < rows; i++)
                                    fprint_line(fp_print, transports_sorted[2], i);
                                break;
                            case 4:
                                transports_sorted[3] = merge_sort(transports, transports_sorted[0], rows, 5);
                                for (i = 0; i < rows; i++)
                                    fprint_line(fp_print, transports_sorted[3], i);
                                break;
                            case 5:
                                break;
                            default:
                                printf("Operation not allowed! Try again.\n\n");
                                break;
                        }
                    }
                    else if (command == 3)
                        break;
                    else
                        printf("Operation not allowed! Try again.\n\n");
                    
                    printf("Operation completed.\n\n");
                    fclose(fp);
                }
                else if (command == 3)
                    break;
                break;
            }
                //Showing data sorted by dates
            case 2:{
                if (transports_sorted[0] == NULL)   //If we have already sorted data in case 1 for printing them in a file, we don't have to sort them again
                    transports_sorted[0] = merge_sort(transports, transports_sorted[0], rows, 2);
                
                printf("\nFile has been sorted by dates:\n");
                for (i = 0; i < rows; i++)
                    print_line(transports_sorted[0], i);    //Showing every elements of sorted data
                printf("\n\n");
                break;
            }
                //Showing data sorted by codes
            case 3:{
                if (transports_sorted[1] == NULL)   //If we have already sorted data in case 1 for printing them in a file, we don't have to sort them again
                    transports_sorted[1] = merge_sort(transports, transports_sorted[1], rows, 3);
                
                printf("\nFile has been sorted by code:\n");
                for (i = 0; i < rows; i++){
                    print_line(transports_sorted[1], i);    //Showing every elements of sorted data
                }
                printf("\n\n");
                break;
            }
                //Showing data sorted by leaving station
            case 4:{
                if (transports_sorted[2] == NULL)   //If we have already sorted data in case 1 for printing them in a file, we don't have to sort them again
                    transports_sorted[2] = merge_sort(transports, transports_sorted[2], rows, 4);
                
                printf("\nFile has been sorted by leaving station:\n");
                for (i = 0; i < rows; i++){
                    print_line(transports_sorted[2], i);    //Showing every elements of sorted data
                }
                printf("\n\n");
                break;
            }
                //Showing data sorted by arriving station
            case 5:{
                if (transports_sorted[3] == NULL)   //If we have already sorted data in case 1 for printing them in a file, we don't have to sort them again
                    transports_sorted[3] = merge_sort(transports, transports_sorted[3], rows, 5);
                
                printf("\nFile has been sorted by arriving station:\n");
                for (i = 0; i < rows; i++){
                    print_line(transports_sorted[3], i);    //Showing every elements of sorted data
                }
                printf("\n\n");
                break;
            }
                //Searching for a particular leaving station
            case 6:{
                printf("\nInsert name of the leaving station you are looking for: ");
                scanf("%s", station_name);
                my_toupper(station_name);   //Converting th estring in uppercase
                if (transports_sorted[2] == NULL)
                    linear_search(station_name, transports, rows);   //Case in which data have not been sorted by leaving station so we have to use a linear search
                else
                    binary_search(station_name, transports_sorted[2], rows); //Case in which data have been sorted by leaving station so we could use a binary search
                break;
            }
                //Terminate operation
            case 0:{
                printf("You are going to terminate execution of this program. Press 0 to confirm or any other character to continue.\n");
                scanf("%*c%c", &command_confirm);
                if (command_confirm == '0')
                    exit(EXIT_SUCCESS);
                else
                    break;
            }
            default:{
                printf("Operation not allowed! Try again.\n\n");
                break;
            }
        }
    }
    exit(EXIT_SUCCESS);
}
//Merge sort wrapper. transport_A is the original array of struct in which are stored the information read from file. transport_B is a copy of transport_A in which the algorithm will stored the sorted data. Operation works for the switch in the merge function in which the algorithm decides which elements of the struct will be compared (2 is for dates, 3 is for codes etc..)
dato* merge_sort(dato *transport_A, dato *transport_B, int rows, int operation){
    int i;
    
    //ynamic allocation for transport_B
    transport_B = (dato*) malloc(rows * sizeof(dato));
    if (transport_B == NULL){
        fprintf(stdin, "ERROR: not enough space in memory!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Copying transport_A in transport_B
    for (i = 0; i < rows; i++)
        *(transport_B + i) = *(transport_A + i);
    
    //Real merge sort function
    merge_sort_r(transport_B, 0, rows-1, operation);
    
    return transport_B;
}
void merge_sort_r(dato *transport, int left, int right, int operation){
    int medium;
    
    //Terminate condition
    if (right - left < 1)
        return;
    
    medium = (left + right)/2;
    merge_sort_r(transport, left, medium, operation);   //Recursively computing left elements
    merge_sort_r(transport, medium+1, right, operation);//Recursively computing right elements
    merge(transport, left, medium, right, operation);   //Merging left and right elements
}
void merge(dato *transport, int left, int medium, int right, int operation){
    int length, i, j, k, flag = 0;  //Flag is used to see if we made some operation in our loop
    dato *transport_tmp = NULL; //Auxiliary array for merging (algorithm not IN LOCO)
    
    //Dynamic allocation from transport_tmp
    length = right - left + 1;
    transport_tmp = (dato*) malloc(length * sizeof(dato));
    if (transport_tmp == NULL){
        fprintf(stdin, "ERROR: not enough space in memory!\n\n");
        exit(EXIT_FAILURE);
    }
    
    i = left;   //i counter to move from left side to medium
    j = medium + 1; //j counter to move from medium+1 to right side
    
    //Loop to move in the tmp array of struct for storing data from the unsorted array of struct
    for (k = 0; k <= right - left; ++k){
        //If we have seen all left elements (left side to medium). Loop, from now on, will always enter in this condition so we just have to copy all the right elements (incrementing j counter)
        if (i > medium){
            transport_tmp[k] = transport[j++];
            flag = 1;   //Flag value is changed to 1
        }
        //If we have seen all rigth elements (medium+1 to right side). Loop, from now on, will always enter in this condition so we just have to copy all the left elements (incrementing i counter)
        else if (j > right){
            transport_tmp[k] = transport[i++];
            flag = 1;   //Flag value is changed to 1
        }
        //We now compare the keys for the current elements of the array of struct and storing in transport_tmp the correct element without loosing original sorted keys (so the agorithm is stable)
        else
            switch (operation){
                case 2:
                    if (strcmp(transport[i].date, transport[j].date) <= 0){ //We check if the two dates are in the lexicographic order (left one comes first than right one)
                        if (strcmp(transport[i].date, transport[j].date) == 0){
                            if (strcmp(transport[i].time_start, transport[j].time_start) <= 0){
                                transport_tmp[k] = transport[i++];
                                flag = 1;
                            }
                            else{
                                transport_tmp[k] = transport[j++];
                                flag = 1;
                            }
                        }
                        else{
                            transport_tmp[k] = transport[i++];
                            flag = 1;
                        }
                    }
                    break;
                case 3:
                    if (strcmp(transport[i].code, transport[j].code) <= 0){ //We check if the two codes are in the lexicographic order (left one comes first than right one)
                        transport_tmp[k] = transport[i++];
                        flag = 1;
                    }
                    break;
                case 4:
                    if (strcmp(transport[i].start, transport[j].start) <= 0){ //We check if the two leaving stations are in the lexicographic order (left one comes first than right one)
                        transport_tmp[k] = transport[i++];
                        flag = 1;
                    }
                    break;
                case 5:
                    if (strcmp(transport[i].end, transport[j].end) <= 0){ //We check if the two arriving stations are in the lexicographic order (left one comes first than right one)
                        transport_tmp[k] = transport[i++];
                        flag = 1;
                    }
                    break;
            }
        
        //If none of the previously operations has been done, we copy in transport_tmp the first value
        if (flag == 0)
            transport_tmp[k] = transport[j++];
        else
            flag = 0;
    }
    //Copying in transport (or transport_B (or also transport_sorted[operation] (sorted array of struct))) the sorted elements in transport_tmp
    for (k = left, i = 0; k <= right; k++, i++)
        transport[k] = transport_tmp[i];
}
void linear_search(char *station_name, dato *transport, int rows){
    int i, flag = 0;
    char string_tmp[MAX_STRING];    //Used to stored the leaving station from the strcut element converted in uppercase (in order to avoid to edit the original one)
    for (i = 0; i < rows; i++){
        strcpy(string_tmp, transport[i].start);
        my_toupper(string_tmp);
        if (strstr(string_tmp, station_name) != NULL){
            if (flag == 0){
                flag = 1;
                printf("Here is what you were looking for:\n");
            }
            print_line(transport, i);
        }
    }
    if (flag == 0)
        printf("Leaving station not found. Try again.\n\n");
    else
        printf("\n\n");
}
void binary_search(char *station_name, dato *transport_sorted, int rows){
    int left, right, medium, i, flag = 0;
    char string_tmp[MAX_STRING];    //Used to stored the leaving station from the strcut element converted in uppercase (in order to avoid to edit the original one)
    left = 0;
    right = rows - 1;
    
    //Binary search algorithm
    while (left <= right){
        medium = (left + right)/2;
        strcpy(string_tmp, transport_sorted[medium].start);
        my_toupper(string_tmp);
        if (strcmp(station_name, string_tmp) < 0)
            right = medium - 1;
        else if (strcmp(station_name, string_tmp) > 0)
            left = medium + 1;
        else{
            printf("Here is what you were looking for:\n");
            for (i = medium; strcmp(transport_sorted[medium].start, transport_sorted[i].start) == 0; --i)
                print_line(transport_sorted, i);
            for (i = medium + 1; strcmp(transport_sorted[medium].start, transport_sorted[i].start) == 0; ++i)
                print_line(transport_sorted, i);
            
            flag = 1;
            break;
        }
    }
    if (flag == 0)
        printf("Leaving station not found. Try again.\n\n");
}
void my_toupper(char *string){
    int i;
    for (i = 0; string[i] != '\0'; i++)
        if (string[i] >= 'a' && string[i] <= 'z')
            string[i] = string[i] - 32;
}
void print_menu(void){
    printf("MENÙ:\n\n");
    printf("1) Printing all data from file.\n");
    printf("2) Showing data sorted by dates.\n");
    printf("3) Showing data sorted by codes.\n");
    printf("4) Showing data sorted by leaving stations.\n");
    printf("5) Showing data sorted by arrival stations.\n");
    printf("6) Search for a particular leaving station.\n");
    printf("0) Terminate execution.\n\n");
    printf("Select your operation: ");
}
void print_line (dato *transports, int i){
    printf("%s %s %s %s %s %s %d.\n", transports[i].code, transports[i].start, transports[i].end, transports[i].date, transports[i].time_start, transports[i].time_end, transports[i].delay);
}
void fprint_line(FILE *fp, dato *transports, int i){
    fprintf(fp, "%s %s %s %s %s %s %d.\n", transports[i].code, transports[i].start, transports[i].end, transports[i].date, transports[i].time_start, transports[i].time_end, transports[i].delay);
}
dato* reading_file (FILE *fp, int *rows){       //Dynamic allocation of memory and saving data from file in an array of struct
    dato *array = NULL;
    int i = 0;
    char line[MAX_LINE];
    
    fscanf(fp, "%d\n", &(*rows));             //Getting dimension of file
    
    //Dynamic memory allocation for array of struct
    array = (dato*) malloc((*rows) * sizeof(dato));
    if (array == NULL){
        fprintf(stderr, "ERROR: not enough space in memory!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Reading all the parameters
    while (fgets(line, MAX_LINE, fp) != NULL){
        sscanf(line, "%s %s %s %s %s %s %d", array[i].code, array[i].start, array[i].end, array[i].date, array[i].time_start, array[i].time_end, &array[i].delay);
        ++i;
    }
    return array;       //return the pointer to array
}
void checking_file (FILE *fp, char *filename){  //Check if a file is opening well
    if (fp == NULL){
        fprintf(stderr, "ERROR: file %s not found!\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
