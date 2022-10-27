//
//  equipArray.h
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#ifndef EQUIPARRAY_H
#define EQUIPARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "invArray.h"

#define EQUIP_SLOT 8

/* ---- DATA ---- */

//ADT for list of weapons in the inventory
typedef struct equipArray_s *equipArray_t;

/* ---- FUNCTIONS ---- */

//CREATE AND FREE FUNCTIONS
equipArray_t equipArrayINIT (void);

//CHECK FUNCTIONS
int checkEquipArray_inUse (equipArray_t equip);
int checkEquipArray_FULL (equipArray_t equip);

//SHOW FUNCTIONS
int getSTATS (equipArray_t equip, invArray_t invArray, int i);
void weaponInUse_SHOW (equipArray_t equip, invArray_t invArray);
int equip_SHOW (equipArray_t equip, invArray_t invArray);

//EDIT USER'S INVENTORY FUNCTIONS
void weapon_ADD (equipArray_t *equip, invArray_t invArray, int wp);
void weapon_REMOVE (equipArray_t *equip, int wp);
void weapon_EDIT (equipArray_t *equip, int wp);

#endif /* EQUIPARRAY_H */
