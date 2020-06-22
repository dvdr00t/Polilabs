//
//  pg.h
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#ifndef PG_H
#define PG_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "equipArray.h"

#define MAX_CODE_LENGTH 7

/* ---- DATA ---- */

//QUASI ADT for users
typedef struct {
    char codeUSER[MAX_CODE_LENGTH];
    char nameUSER[MAX_STRING_LENGTH];
    char classUSER[MAX_STRING_LENGTH];
    equipArray_t equip;
    stat_t stats;
} pg_t;

/* ---- FUNCTIONS ---- */

//INPUT AND OUTPUT FUNCTIONS
int pg_READ (FILE *fp, pg_t *user);
void pg_PRINT (FILE *fp, pg_t user, invArray_t invArray);

//STATS FUNCTIONS
pg_t computeSTATS (pg_t *user, invArray_t invArray);

//EQUIPMENT FUNCTIONS
void pgEquip_UPDATE (pg_t *userTMP, invArray_t invArray);

#endif /* PG_H */
