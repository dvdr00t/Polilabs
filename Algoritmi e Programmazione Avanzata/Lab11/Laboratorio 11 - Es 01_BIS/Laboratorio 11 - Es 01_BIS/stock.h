//
//  stock.h
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef STOCK_H
#define STOCK_H

#include <stdio.h>
#include <stdlib.h>
#include "ITEM.h"

/* --- DATA --- */

typedef struct data_s *data_t;
typedef struct node_s *node_t;
typedef node_t LINK;

/* --- FUNCTIONS --- */

//Create and free
data_t list_INIT (void);
void list_FREE (data_t set);
void list_FREE_R (LINK x);

//Input and output
void file_READ (data_t *set, char *filename);
void list_SHOW (data_t *set);

//List operations
void stock_READ (FILE *fp, LINK x, int N_exchanges);
void stock_SHOW (LINK x);
LINK stock_NEW (data_t *set, char *code);
LINK newNode (char *code, LINK next);

void date_SEARCH_bySTOCK (LINK x, date_t date);
LINK stock_SEARCH (data_t set, char *code);


//Search interval operation
void maxmin_SEARCH (LINK x, date_t date_A, date_t date_B);

//Balance
float balanceValue_COMPUTE(LINK x);
void balance (LINK x);

#endif /* STOCK_H */
