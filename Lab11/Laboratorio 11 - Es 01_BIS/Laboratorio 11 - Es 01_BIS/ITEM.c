//
//  ITEM.c
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "ITEM.h"

/* --- DATA --- */

//STOCK INFORMATION
struct stock_s {
    
    //STOCK ID
    char code[MAX_CODE_LENGTH];
    
    //ADT BST
    BST root;
};


/* --- FUNCTIONS --- */

//Create and free
ITEM ITEM_INIT(char *code) {
    
    //DYNAMIC ALLOCATION
    ITEM stock = (ITEM) malloc(sizeof(struct stock_s));
    if (stock == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING STOCK
    strcpy(stock->code, code);
    stock->root = BSTinit();
    
    return stock;
}
void ITEM_FREE (ITEM stock) {
    BSTfree(stock->root);
}

//Input and output
void ITEM_READ (FILE *fp, ITEM stock, int N_exchanges) {
    
    //READING STOCK EXHANGES INFORMATION
    quote_t stock_exchange;
    link x;
    for (int i = 0; i < N_exchanges; i++) {
        
        //INITIALIZING STOCK EXCHANGE INFORMATIONS
        exchange_INIT(&stock_exchange);
        
        //READING STOCK EXCHANGE INFROMATIONS
        exchange_READ(fp, &stock_exchange);
        
        //SEARCHING FOR AN EXISTING NODE IN QUOTATION BST
        x = BSTsearch(stock->root, stock_exchange.date);
        
        //LEAF INSERT IF THE NODE IS NEW (i.e. DATE HAS NOT ALREADY BEEN INSERTED)
        if (x == sentinel_CHECK(stock->root))
            BSTinsert_leaf(stock->root, stock_exchange);
        
        //UPLOAD BST NODE
        else
            BSTupload (x, stock_exchange);
    }
}
void ITEM_SHOW (ITEM stock) {
    BST_SHOW (stock->root);
}

//Operation
void date_SEARCH_byITEM (ITEM stock, date_t date) {
    link x = BSTsearch(stock->root, date);
    if (x == NULL)
        fprintf(stdout, "No quotation found for this date!");
    else {
        fprintf(stdout, "\n\nQuotation found:\n");
        quotation_SHOW(x);
    }
    
    printf("\n\n");
}
int BST_count (ITEM stock) {
    return count(stock->root);
}
void maxmin_SEARCH_byITEM (ITEM stock, date_t date_A, date_t date_B) {
    maxmin_SEARCH_byTREE (stock->root, date_A, date_B);
}

//KEY operations
int KEYcompare (char *code_A, char *code_B) {
    if (strcmp(code_A, code_B) < 0)
        return 1;
    if (strcmp(code_A, code_B) > 0)
        return 2;
    if (strcmp(code_A, code_B) == 0)
        return 0;
    
    return -1;
}
char* KEYget(ITEM stock) {
    return stock->code;
}

//Balance
float balanceValue_COMPUTE_byITEM (ITEM stock) {
    return balanceValue_COMPUTE_byTREE(stock->root);
}
void balance_byITEM(ITEM stock) {
    balance_byTREE(stock->root);
}
