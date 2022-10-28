//
//  pg.c
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include "pg.h"
/* ---- DATA ---- */


/* ---- FUNCTIONS ---- */
//INPUT AND OUTPUT FUNCTIONS
int pg_READ (FILE *fp, pg_t *user) {
    static char code[MAX_CODE_LENGTH] = "VOID";
    
    //READING CODE, NAME AND CLASS
    fscanf(fp, "%s %s %s ", user->codeUSER, user->nameUSER, user->classUSER);
    
    //CHECKING IF THE BUFFER HAS ARRIVED AT THE END OF THE FILE AND KEEP READING THE SAME INFORMATION
    //If the information (CODE, which is unique in the input FILE) is the same
    if (strcmp(user->codeUSER, code) == 0)
        return EOF; //RETURNING EOF
    
    //If the information is not the same
    else {
        //UPLOADING LAST INFORMATION (code, which is unique in the input FILE)
        strcpy(code, user->codeUSER);
        
        //READING STATS
        inv_statREAD(fp, &user->stats);
        
        //RETURNING VALUE
        return 1;
    }
    
}
void pg_PRINT(FILE *fp, pg_t user, invArray_t invArray) {
    
    //CHECKING IF THE USER HAS A WEAPON IN USE OR NOT
    //If the user has a weapon in use
    if (checkEquipArray_inUse(user.equip)) {
        
        //COMPUTING NEW STATS
        pg_t userEQUIPPED = computeSTATS(&user, invArray);
        
        //SHOWING DATA
        printf("%s %s %s ", user.codeUSER, user.nameUSER, user.classUSER);
        inv_statSHOW(stdout, &userEQUIPPED.stats);  //Modified data due to the weapon
        
        //SHOWING INFORMATION ABOUT WEAPON IN USE
        printf(" --- [WEAPON IN USE: ");
        weaponInUse_SHOW(user.equip, invArray);
        printf("]");
    }
    //If the user has not a weapon in use
    else {
        //SHOWING DATA
        printf("%s %s %s ", user.codeUSER, user.nameUSER, user.classUSER);
        inv_statSHOW(stdout, &user.stats);
    }
}

//STATS FUNCTION
pg_t computeSTATS (pg_t *user, invArray_t invArray) {
    pg_t userEQUIPPED;
    
    //CHECK IF USER STATS ARE GREATER THAN ZERO OR NOT
    userEQUIPPED.stats.hp = user->stats.hp + getSTATS(user->equip, invArray, 0);
    if (userEQUIPPED.stats.hp < 0)
        userEQUIPPED.stats.hp = 0;
    
    userEQUIPPED.stats.mp = user->stats.mp + getSTATS(user->equip, invArray, 1);
    if (userEQUIPPED.stats.mp < 0)
        userEQUIPPED.stats.mp = 0;
    
    userEQUIPPED.stats.atk = user->stats.atk + getSTATS(user->equip, invArray, 2);
    if (userEQUIPPED.stats.atk < 0)
        userEQUIPPED.stats.atk = 0;
    
    userEQUIPPED.stats.def = user->stats.def + getSTATS(user->equip, invArray, 3);
    if (userEQUIPPED.stats.def < 0)
        userEQUIPPED.stats.def = 0;
    
    userEQUIPPED.stats.mag = user->stats.mag + getSTATS(user->equip, invArray, 4);
    if (userEQUIPPED.stats.mag < 0)
        userEQUIPPED.stats.mag = 0;
    
    userEQUIPPED.stats.spr = user->stats.spr + getSTATS(user->equip, invArray, 5);
    if (userEQUIPPED.stats.spr < 0)
        userEQUIPPED.stats.spr = 0;
    
    return userEQUIPPED;
}

//EQUIPMENT FUNCTIONS
void pgEquip_UPDATE (pg_t *userTMP, invArray_t invArray) {
    
    //SELECTING OPERATION
    int command;    //Switching operator
    int wp;         //Index of weapon in the collection of weapons
    
    
    printf("\nEQUIPMENT:\n");
    printf("1 - ADD weapon\n");
    printf("2 - REMOVE weapon\n");
    printf("3 - EDIT weapon in use\n");
    printf("4 - EXIT\n\n");
    printf("Select operation: ");
    scanf("%d", &command);
    
    switch (command) {
            
            /*
             *
             *      { WE SUPPOSE TO CHOOSE CORRECT VALUES IN ORDER TO OMIT TO CHECK INPUT }
             *
             */
            
            //ADD A WEAPON
        case 1: {
            
            //CHECKING IF THERE IS SPACE IN THE USER'S EQUIPMENT
            if (!checkEquipArray_FULL(userTMP->equip)) {
                
                //SHOWING USER INFORMATION
                printf("\nUSER INFORMATIONS AND WEAPON IN USE:\n");
                pg_PRINT(stdout, *userTMP, invArray);
                
                printf("\nUSER WEAPONS:\n");
                equip_SHOW(userTMP->equip, invArray);
                
                //SHOWING INVENTORY
                printf("\nINVENTORY:\n");
                invArray_SHOW(invArray);
                
                //SELECTING WEAPON TO ADD
                printf("\nSelect weapon to add in the user's equipment: ");
                scanf("%d", &wp);
                
                //ADDING WEAPON
                weapon_ADD (&userTMP->equip, invArray, wp);
                
                printf("Operation completed!\n\n");
                return;
            }
        }
            
            //REMOVE A WEAPON
        case 2: {
            
            //CHECKING IF THE USER'S EQUIPEMENT IS EMPTY
            if (equip_SHOW(userTMP->equip, invArray)) {
                
                //SHOWING USER INFORMATION
                printf("\nUSER INFORMATIONS AND WEAPON IN USE:\n");
                pg_PRINT(stdout, *userTMP, invArray);
                
                //SELECTING WEAPON TO REMOVE
                printf("\nSelect weapon to remove from user's equipment: ");
                scanf("%d", &wp);
                
                //REMOVING WEAPON
                weapon_REMOVE (&userTMP->equip, wp);
                
                printf("Operation completed!\n\n");
                return;
            }
            else
                return;
        }
            
            //EDIT WEAPON IN USE
        case 3: {
            
            //CHECKING IF THE USER'S EQUIPEMENT IS EMPTY
            if (equip_SHOW(userTMP->equip, invArray)) {
                
                //SHOWING USER INFORMATION
                printf("\nUSER INFORMATIONS AND WEAPON IN USE:\n");
                pg_PRINT(stdout, *userTMP, invArray);
                
                //SELECTING WEAPON TO REMOVE
                printf("\nSelect weapon to set as weapon in use: ");
                scanf("%d", &wp);
                
                //REMOVING WEAPON
                weapon_EDIT (&userTMP->equip, wp);
                
                printf("Operation completed!\n\n");
                return;
            }
            else
                return;
        }
            
            //EXIT
        case 4: {
            printf("Back to the MENU\n\n");
            return;
        }
            
            //DEFAULT
        default: {
            printf("ERROR!\n\n");
            return;
        }
    }
}
