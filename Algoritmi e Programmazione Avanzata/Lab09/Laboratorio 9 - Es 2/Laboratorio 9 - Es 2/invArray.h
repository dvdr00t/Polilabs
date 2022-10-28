//
//  invArray.h
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#ifndef INVARRAY_H
#define INVARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "inv.h"

/* ---- DATA ---- */

//ADT for collection of weapons
typedef struct invArray_s *invArray_t;

/* ---- FUNCTIONS ---- */
//CREATE AND FREE FUNCTIONS
invArray_t invArray_INIT (void);

//INPUT AND OUPUT FUNCTIONS
void invArrayRead_FILE (invArray_t invArray, char *filename);

//SHOW FUNCTIONS
void invArray_SHOW (invArray_t invArray);
int readSTATS (invArray_t invArray, int index, int stat);
void weapon_PRINT (invArray_t invArray, int index);


#endif /* INVARRAY_H */
