//
//  inv.h
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#ifndef INV_H
#define INV_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STRING_LENGTH 100


/* ---- DATA ---- */

//QUASI ADT for stats
typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} stat_t;

//QUASI ADT for weapons in the inventory
typedef struct {
    char nameWeapon[MAX_STRING_LENGTH];
    char typeWeapon[MAX_STRING_LENGTH];
    stat_t stats;
} inv_t;

/* ---- FUNCTIONS ---- */

//INPUT AND OUTPUT for stats
void inv_statREAD (FILE *fp, stat_t *stat);
void inv_statSHOW (FILE *fp, stat_t *stat);

//INPUT AND OUTPUT for inventory
void inv_invREAD (FILE *fp, inv_t *inv);
void inv_invSHOW (FILE *fp, inv_t *inv);


#endif /* INV_H */
