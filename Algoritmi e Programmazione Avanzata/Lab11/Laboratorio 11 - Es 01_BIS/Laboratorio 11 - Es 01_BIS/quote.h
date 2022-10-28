//
//  quote.h
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef QUOTE_H
#define QUOTE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "time.h"

/* --- DATA --- */

//DAILY STOCK EXCHANGE (QUASI ADT)
typedef struct {
    
    //NODE IDENTIFICATOR
    date_t date;
    
    //QUOTATION VALUES
    float numerator;    //sum(Vi*Ni) = V1*N1 + V2*N2 + ...
    float denominator;  //sum(Ni) = N1 + N2 + ...
} quote_t;

//COLLECTION OF STOCK EXCHANGES (BST)
typedef struct BinarySearchTree* BST;
typedef struct BSTnode* link;

/* --- FUNCTIONS --- */

//Create and free
BST BSTinit (void);
void exchange_INIT (quote_t *exchange);
void BSTfree (BST bst);
void tree_FREE (link root, link z);
quote_t exchange_SET_NULL (void);

//Input and output
void exchange_READ (FILE *fp, quote_t *exchange);
void BST_SHOW (BST bst);
void BST_SHOW_R (link root, link z);
void quotation_SHOW (link x);
link sentinel_CHECK (BST bst);

//Operation
void BSTinsert_leaf (BST bst, quote_t exchange);
void BSTupload (link x, quote_t exchange);
link BSTsearch (BST bst, date_t date);
link BSTsearch_R (link root, date_t date, link z);
link insert (link h, quote_t exchange, link z);
link NEW (quote_t exchange, link father, link left, link right);
int count (BST bst);
int count_R (link x, link z);
void maxmin_SEARCH_byTREE (BST bst, date_t date_A, date_t date_B);
void maxmin_SEARCH_byTREE_R (link x, link z, date_t date_A, date_t date_B, quote_t *max, quote_t *min);
float balanceValue_COMPUTE_byTREE (BST bst);
float height_COMPUTE (link x, link z);
float short_COMPUTE (link x, link z);
void balance_byTREE (BST bst);
link balance_byTREE_R (link x, link z);
link partR (link x, link z, int value);
link rotR (link x);
link rotL (link x);
#endif /* quote_h */
