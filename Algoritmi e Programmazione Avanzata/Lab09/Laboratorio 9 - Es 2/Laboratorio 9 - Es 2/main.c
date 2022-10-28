//
//  main.c
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgList.h"         //LIST OF USER'S MOD
#include "invArray.h"       //WEAPON'S MOD
#include "pg.h"             //USER'S MOD

#define MAX_FILENAME_LENGTH 30

pgList_t pgList;        //LIST OF USERS
invArray_t invArray;    //COLLECTION OF WEAPONS

int getOPERATION (void);
void showMENU (void);
int main(int argc, const char *argv[]) {
    
    //CHECKING TERMINAL PARAMETERS
    if (argc != 1){
        fprintf(stderr, "ERROR: insert only program name!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //CREATING LIST OF USERS AND ARRAY OF WEAPONS
    pgList = pgList_INIT();
    invArray = invArray_INIT();
    
    //READING DATA FROM FILE
    //Reading users
    char pgFilename[MAX_FILENAME_LENGTH];
    fprintf(stdout, "INSERT USERS FILE NAME: ");
    fscanf(stdin, "%s", pgFilename);
    pgRead_FILE(pgList, pgFilename);
    
    //Reading weapons
    char invFilename[MAX_FILENAME_LENGTH];
    fprintf(stdout, "INSERT INVENTORY FILE NAME: ");
    fscanf(stdin, "%s", invFilename);
    invArrayRead_FILE(invArray, invFilename);
    
    //MENU
    int end = 0;
    while (!end) {
        showMENU();
        printf("\nSelect operation: ");
        end = getOPERATION();
    }
    
}
void showMENU (void) {
    printf("---MENU---\n");
    printf("1 - Show the whole list of users\n");
    printf("2 - Show the whole list of weapons\n");
    printf("3 - Add a new user to the list\n");
    printf("4 - Delete an existing user from the list\n");
    printf("5 - Edit user's equipment\n");
    printf("0 - TERMINATE EXECUTION\n");
}
int getOPERATION (void) {
    int command;    //Switch operator
    fscanf(stdin, "%d", &command);
    
    switch (command) {
            
            //CASE 1: SHOW THE WHOLE LIST OF USERS
        case 1: {
            pgList_SHOW(pgList, invArray);
            return 0;
        }
            
            //CASE 2: SHOW THE WHOLE COLLECTIONS OF WEAPONS
        case 2: {
            invArray_SHOW(invArray);
            return 0;
        }
            
            //CASE 3: CREATING A NEW USER
        case 3: {
            
            //Creating a new user and check if it already exista
            pg_t userNEW = getUser_INFORMATION(pgList);
            
            //If the new user does not exist yet
            if (strcmp(userNEW.nameUSER, "NOTVALID") != 0) {
                
                //ADDING THE USER TO THE LIST
                pgList_INSERT(pgList, userNEW);
                printf("Operation completed!\n\n");
                return 0;
            }
            //If the user exists
            else
                return 0;
        }
            
            //CASE 4: DELETING AN EXISTING USER
        case 4: {
            
            //CHECKING IF THE LIST IS EMPTY OR NOT
            if (!checkHeadList(pgList)) {
                printf("The list is empty! You can not remove any users.\n\n");
                return 0;
            }
            
            //SHOWING THE LIST TO THE USER
            pgList_SHOW(pgList, invArray);
            
            //GETTING CODE OF USER
            printf("INSERT CODE: ");
            char code[MAX_CODE_LENGTH];
            scanf("%s", code);
            
            //REMOVING THE NODE FROM THE LIST
            link nodeTMP = pgList_REMOVE(pgList, code);
            
            //If we have not found the node
            if (nodeTMP == NULL) {
                printf("USER NOT FOUND!\n\n");
                return 0;
            }
            //If we have found the node
            else {
                printf("USER FOUND! Here's its informations:\n");
                
                //SHOWING INFORMATION ABOUT THE USER FOUND
                pg_PRINT(stdout, userACQUISITION(nodeTMP), invArray);
                
                //DECIDING IF WE REALLY WANT TO DELETE IT OR NOT
                printf("\nAre you sure you want to delete this user?\n");
                printf("1) I am sure.\n");
                printf("0) NO, I dont want to delete it anymore!\n\n");
                printf("Select your operation: ");
                scanf("%d", &command);
                
                //IF WE REALLY WANT TO DELETE IT
                if (command == 1){
                    
                    //DEALLOCATING THE NODE EXTRACTED FROM THE LIST
                    free(nodeTMP);
                    
                    printf("Operation completed!\n\n");
                    return 0;
                }
                
                //IF WE DO NOT WANT TO DELETE THE USER EXTRACTED
                else {
                    
                    //RE-INSERTING THE USER TO THE LIST
                    pgList_INSERT(pgList, userACQUISITION(nodeTMP));
                    printf("The user has not been deleted.\n\n");
                    return 0;
                }
                
            }
            
        }
            
            //CASE 5: UPDATING USER'S EQUIPMENT
        case 5: {
            
            //CHECKING IF THE LIST IS EMPTY OR NOT
            if (!checkHeadList(pgList)) {
                printf("The list is empty! You can not update any user's equipment.\n\n");
                return 0;
            }
            
            //SHOWING THE LIST TO THE USER
            pgList_SHOW(pgList, invArray);
            
            //GETTING CODE OF THE USER
            printf("INSERT CODE: ");
            char code[MAX_CODE_LENGTH];
            scanf("%s", code);
            
            //GETTING THE NODE
            link nodeTMP = searchUSER(pgList, code);
            
            //SEARCH MISS
            if (nodeTMP == NULL){
                printf("USER NOT FOUND!\n\n");
                return 0;
            }
            
            //SEARCH HIT
            else {
                pgEquip_UPDATE(PointerUserACQUISITION(nodeTMP), invArray);
                return 0;
            }
            
            
        }
            
            //CASE 0: TERMINATE EXECUTION
        case 0: {
            char confirmCommand;
            printf("You are going to terminate the execution of this program. Press 0 if you really want to terminate execution or press any other button to go back to the MENU.\n\n");
            scanf("%*c%c", &confirmCommand);
            if (confirmCommand == '0') {
                pgList_FREE(pgList);
                exit(EXIT_SUCCESS);
            }
            else
                return 0;
        }
            
            //DEFAULT: ERROR OPERATION
        default: {
            printf("This operation is not allowed!\n\n");
            return 0;
        }
    }
}
