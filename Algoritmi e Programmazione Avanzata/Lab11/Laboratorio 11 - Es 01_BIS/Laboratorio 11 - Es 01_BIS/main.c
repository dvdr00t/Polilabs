//
//  main.c
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stock.h"
#define S 1 //ACCEPT VALUE FOR BALANCING TREE (HEIGHT OVER SHORTEST PATH)
#define MAX_FILENAME_LENGTH 30


int operation_GET(data_t *set);
void menu_SHOW(void);
int main(int argc, const char * argv[]) {
    
    //CHECKING TERMINAL CONDITIONS
    if (argc != 1) {
        fprintf(stderr, "ERROR: insert only program name.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING VARIABLE USED
    data_t set = list_INIT();
    int end = 0;
    
    //MENU IMPLEMENTATION
    while (!end) {
        menu_SHOW();
        end = operation_GET(&set);
    }
    
    //FREE
    list_FREE(set);
    
    exit(EXIT_SUCCESS);
}
void menu_SHOW(void) {
    fprintf(stdout, "--- MENU ---\n\n");
    fprintf(stdout, "1 - Insert new FILE.\n");
    fprintf(stdout, "2 - Search stock.\n"); //TITOLO
    fprintf(stdout, "3 - Search stock quotation.\n");   //QUOTAZIONE IN BORSA
    fprintf(stdout, "4 - Search best and worst stock quotations.\n");    //MASSIMO E MINIMO VALORE ASSUNTO DALLA QUOTAZIONE IN BORSA
    fprintf(stdout, "5 - Balance stock BST.\n");
    fprintf(stdout, "0 - QUIT EXECUTION.\n\n");
}
int operation_GET(data_t *set) {
    
    //GETTING OPERATION
    int command;
    fprintf(stdout, "Select: ");
    fscanf(stdin, "%d", &command);
    
    //SWITCHING OPERATION
    switch (command) {
            
            //CASE 1: INSERTION OF A NEW FILE
        case 1: {
            
            //READING FILE NAME
            char filename[MAX_FILENAME_LENGTH];
            fprintf(stdout, "Insert new FILE name: ");
            fscanf(stdin, "%s", filename);
            
            //READING FILE DATA
            file_READ(set, filename);
            
            //PRINT DATA
            list_SHOW(set);
            
            return 0;
        }
            
            //CASE 2: SEARCH OF A STOCK
        case 2: {
            
            //READING STOCK NAME
            char code[MAX_CODE_LENGTH];
            fprintf(stdout, "Insert code: ");
            fscanf(stdin, "%s", code);
            
            //SEARCH STOCK IN THE LIST
            LINK x = stock_SEARCH(*set, code);
            if (x == NULL) {
                fprintf(stdout, "Stock not found!\n\n");
                return 0;
            }
            else
                stock_SHOW(x);
            
            printf("\n\n");
            return 0;
        }
            
            //CASE 3: SEARCH A STOCK IN A PARTICULAR DATE
        case 3: {
            
            //READING STOCK NAME
            char code[MAX_CODE_LENGTH];
            fprintf(stdout, "Insert code: ");
            fscanf(stdin, "%s", code);
            
            //SEARCH STOCK IN THE LIST
            LINK x = stock_SEARCH(*set, code);
            if (x == NULL) {
                fprintf(stdout, "Stock not found!\n\n");
                return 0;
            }
            else {
                
                //READING DATE
                date_t date;
                fprintf(stdout, "Insert date in the format [yyyy/mm/dd]: ");
                date_READ(stdin, &date);
                
                //SEARCH DATE IN THE LIST
                date_SEARCH_bySTOCK(x, date);
                return 0;
            }
            
        }
            
            //CASE 4: SEARCH MAXIMUM AND MINIMUM VALUES OF QUOTATIONS IN AN INTERVAL OF DATES
        case 4: {
            
            //READING STOCK NAME
            char code[MAX_CODE_LENGTH];
            fprintf(stdout, "Insert code: ");
            fscanf(stdin, "%s", code);
            
            //SEARCH STOCK IN THE LIST
            LINK x = stock_SEARCH(*set, code);
            if (x == NULL) {
                fprintf(stdout, "Stock not found!\n\n");
                return 0;
            }
            else {
                
                //READING COMMAN
                fprintf(stdout, "Search maximum and minimum quotation value in:\n1 - Anytime\n2 - Between two dates given\n0 - Exit\n\nSelect: ");
                fscanf(stdin, "%d", &command);
                
                //SEARCH MAXIMUM AND MINIMUM IN THE WHOLE TREE
                if (command == 1) {
                    date_t date_A, date_B;
                    date_A.year = INT_MIN;
                    date_B.year = INT_MAX;
                    maxmin_SEARCH(x, date_A, date_B);
                    printf("\n\n");
                    return 0;
                }
                
                //SEARCH MAXIMUM AND MINIMUM BETWEEN TWO DATES IN THE TREE
                else if (command == 2) {
                    
                    //READING DATES
                    date_t date_A, date_B;
                    fprintf(stdout, "Insert first date in the format [yyyy/mm/dd]: ");
                    date_READ(stdin, &date_A);
                    fprintf(stdout, "Insert second date in the format [yyyy/mm/dd]: ");
                    date_READ(stdin, &date_B);
                    
                    maxmin_SEARCH(x, date_A, date_B);
                    
                    printf("\n\n");
                    return 0;
                }
                else
                    return 0;
            }
            
            
            return 0;
        }
            
            //CASE 5: BALANCE BST
        case 5: {
            //READING STOCK NAME
            char code[MAX_CODE_LENGTH];
            fprintf(stdout, "Insert code: ");
            fscanf(stdin, "%s", code);
            
            //SEARCHING STOCK IN THE LIST
            LINK x = stock_SEARCH(*set, code);
            if (x == NULL) {
                fprintf(stdout, "Stock not found!\n\n");
                return 0;
            }
            else {
                
                //CHECKING IS THE TREE HAS TO BE BALANCED
                if (balanceValue_COMPUTE(x) > S) {
                    
                    //BALANCE TREE
                    balance(x);
                    fprintf(stdout, "TREE HAS BEEN BALANCED\n\n");
                }
                else
                    fprintf(stdout, "TREE IS ALREADY BALANCED\n\n");
            }
            
            return 0;
        }
            
            //CASE 0: QUIT EXECUTION
        case 0: {
            char command_confirm;
            printf("Are you sure you want to quit execution?\n Press 0 if you want to go back to the MENU.\n Press any other button to QUIT EXECUTION.\n\n");
            scanf("%*c%c", &command_confirm);
            if (command_confirm != '0') {
                printf("\n\n...QUITTING EXECUTION...\n\n");
                return 1;
            }
            return 0;
        }
    }
    
    return -1;
}
