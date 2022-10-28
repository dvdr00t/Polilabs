//
//  invArray.c
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 17/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include "invArray.h"

/* ---- DATA ---- */

struct invArray_s {
    int nInv;   //Numbers of weapons available read from file
    inv_t *arrayInv;    //Pointer to the array of weapons read from file and dynamic allocated
};

/* ---- FUNCTIONS ---- */

//CREATE AND FREE FUNCTIONS
invArray_t invArray_INIT (void) {
    invArray_t invArray;
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY
    invArray = malloc(sizeof(invArray_t));
    if (invArray == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING THE ARRAY
    invArray->nInv = 0;
    invArray->arrayInv = NULL;
    
    return invArray;
}

//INPUT AND OUTPUT FUNCTIONS
void invArrayRead_FILE (invArray_t invArray, char *filename) {
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to open %s FILE.\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    //GETTING NUMBERS OF WEAPONS AVAILABLE
    fscanf(fp, "%d", &invArray->nInv);
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY
    invArray->arrayInv = (inv_t *) malloc(invArray->nInv * sizeof(inv_t));
    if (invArray->arrayInv == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING DATA FROM FILE
    int i;
    for (i = 0; i < invArray->nInv; i++)
        inv_invREAD(fp, &invArray->arrayInv[i]);
}


//SHOW FUNCTIONS
void invArray_SHOW (invArray_t invArray) {
    int i;
    
    printf("\nTHIS IS THE WHOLE LIST OF WEAPONS IN THE INVENTORY:\n");
    
    //SHOWING THE WHOLE LIST OF WEAPONS IN THE INVENTORY
    for (i = 0; i < invArray->nInv; i++) {
        printf("%d - ", i+1);
        inv_invSHOW(stdout, &invArray->arrayInv[i]);
        printf("\n");
    }
    
    printf("\n\n");
}
int readSTATS (invArray_t invArray, int index, int stat) {
    
    index = index - 1;
    //RETURNING THE VALUE VALUE OF A PARTICULAR STAT (HP, MP, ATK, DEF, MAG or SPR)
    if (stat == 0)
        return invArray->arrayInv[index].stats.hp;
    if (stat == 1)
        return invArray->arrayInv[index].stats.mp;
    if (stat == 2)
        return invArray->arrayInv[index].stats.atk;
    if (stat == 3)
        return invArray->arrayInv[index].stats.def;
    if (stat == 4)
        return invArray->arrayInv[index].stats.mag;
    if (stat == 5)
        return invArray->arrayInv[index].stats.spr;
    
    return -1;
}
void weapon_PRINT (invArray_t invArray, int index) {
    
    //SHOW INFORMATION ABOUT A PARTICULAR WEAPON IN THE INVENTORY
    inv_invSHOW(stdout, &invArray->arrayInv[index-1]);
}
