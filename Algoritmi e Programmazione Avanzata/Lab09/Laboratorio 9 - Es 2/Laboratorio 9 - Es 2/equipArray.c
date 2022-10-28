//
//  equipArray.c
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include "equipArray.h"

/* ---- DATA ---- */

struct equipArray_s {
    int index_inUse;
    int wpN;
    int wpArray[EQUIP_SLOT];
};

/* ---- FUNCTIONS ---- */

//CREATE AND FREE FUNCTIONS
equipArray_t equipArrayINIT (void) {
    
    equipArray_t equip;
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY
    equip = (struct equipArray_s*) malloc(1 * sizeof(struct equipArray_s));
    if (equip == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //SETTING USER INVENTOR TO VOID
    int i;
    for (i = 0; i < EQUIP_SLOT; i++) {
        equip->wpArray[i] = EOF;
    }
    equip->index_inUse = -1;
    equip->wpN = 0;
    
    return equip;
}

//CHECK FUNCTIONS
int checkEquipArray_inUse (equipArray_t equip) {
    if (equip->index_inUse == -1)
        return 0;
    return 1;
}
int checkEquipArray_FULL (equipArray_t equip) {
    if (equip->wpN == EQUIP_SLOT - 1)
        return 1;
    return 0;
}

//SHOW FUNCTIONS
int getSTATS (equipArray_t equip, invArray_t invArray, int i) {
    
    //RETURN THE VALUE OF A PARTICULAR STAT (HP, MP, ATK, DEF, MAG or SPR)
    if (i == 0)
        return readSTATS(invArray, equip->wpArray[equip->index_inUse], i);
    if (i == 1)
        return readSTATS(invArray, equip->wpArray[equip->index_inUse], i);
    if (i == 2)
        return readSTATS(invArray, equip->wpArray[equip->index_inUse], i);
    if (i == 3)
        return readSTATS(invArray, equip->wpArray[equip->index_inUse], i);
    if (i == 4)
        return readSTATS(invArray, equip->wpArray[equip->index_inUse], i);
    if (i == 5)
        return readSTATS(invArray, equip->wpArray[equip->index_inUse], i);
    return -1;
}
void weaponInUse_SHOW (equipArray_t equip, invArray_t invArray) {
    int index = equip->wpArray[equip->index_inUse];
    weapon_PRINT(invArray, index);
}
int equip_SHOW (equipArray_t equip, invArray_t invArray) {
    int i;
    int k = 1;
    
    //CHECK IF THE USER?S INVENTORY IS EMPTY
    if (equip->wpN == 0) {
        printf("User's equipment is empty!\n\n");
        return 0;
    }
    
    //IF THE USER'S INVENTORY IS NOT EMPTY
    for (i = 0; i < EQUIP_SLOT; i++) {
        
        //SHOW EVERY INFORMATION ABOUT EVERY WEAPON INT HE INVENTORY
        if (equip->wpArray[i] != -1) {
            printf("%d - ", k++);
            weapon_PRINT(invArray, equip->wpArray[i]);
            printf("\n");
        }
    }
    
    return 1;
}

//EDIT USER'S INVENTORY FUNCTIONS
void weapon_ADD (equipArray_t *equip, invArray_t invArray, int wp) {
    int i;
    
    //ADD A WEAPON IN THE FIRST EMPTY SLOT
    for (i = 0; i < EQUIP_SLOT; i++) {
        if ((*equip)->wpArray[i] == -1) {
            (*equip)->wpArray[i] = wp;  //Uploading weapon
            ((*equip)->wpN)++;  //Uploading weapons counter
            return;
        }
    }
}
void weapon_REMOVE (equipArray_t *equip, int wp) {
    int i;
    int count = 0;  //We use count in order to count how many weapons we have seen in the inventory. This is due to the fact that when we print the user's inventory we use indices from 1 to wpN but we don't know if the "i"-weapon selected is in the "i"-position in the inventory
    
    //REMOVE THE WEAPON SELECTED BY THE USER
    for (i = 0; i < EQUIP_SLOT; i++){
        if ((*equip)->wpArray[i] != -1)
            ++count;
        
        //WHEN WE REACH THE EXACT WEAPON WE WERE LOOKING FOR
        if (count == wp) {
            (*equip)->wpArray[i] = -1;  //Uploading weapon
            ((*equip)->wpN)--;  //Uploading number of weapons
            return;
        }
    }
    return;
}
void weapon_EDIT (equipArray_t *equip, int wp) {
    
    //UPLOADING INDEX OF WEAPON IN USE
    (*equip)->index_inUse = (wp-1);
}
