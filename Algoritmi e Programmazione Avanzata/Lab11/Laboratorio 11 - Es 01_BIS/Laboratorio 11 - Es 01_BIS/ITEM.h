//
//  ITEM.h
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quote.h"

#define MAX_CODE_LENGTH 20

/* --- DATA --- */

typedef struct stock_s *ITEM;

/* --- FUNCTIONS --- */

//Create and free
ITEM ITEM_INIT(char *code);
void ITEM_FREE (ITEM stock);

//Input and output
void ITEM_READ (FILE *fp, ITEM stock, int N_exchanges);
void ITEM_SHOW (ITEM stock);

//Operation
void date_SEARCH_byITEM (ITEM stock, date_t date);
void maxmin_SEARCH_byITEM (ITEM stock, date_t date_A, date_t date_B);
int BST_count (ITEM stock);

//KEY operations
int KEYcompare (char *code_A, char *code_B);
char* KEYget(ITEM stock);

//Balance
float balanceValue_COMPUTE_byITEM (ITEM stock);
void balance_byITEM(ITEM stock);

#endif /* ITEM_H */
