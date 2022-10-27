//
//  stock.c
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "stock.h"

/* --- DATA --- */

//NODE IN THE LIST
struct node_s {
    
    //STOCK INFORMATION
    ITEM stock;
    
    //LINK WITH THE NEXT ELEMENT IN THE LIST
    LINK next;
};

//COLLECTION OF STOCKS
struct data_s {
    LINK head;  //EMPTY LIST
    int N_stocks;
};


/* --- FUNCTIONS --- */

//Create and free
data_t list_INIT (void) {
    
    //DYNAMIC ALLOCATION
    data_t set = NULL;
    set = (data_t) malloc(sizeof(struct data_s));
    if (set == NULL) {
        fprintf(stderr, "ERROR: some errros occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZATION
    set->head = NULL;   //INITIALIZING LIST
    set->N_stocks = 0;  //SET NUMBER OF ELEMENTS IN THE LIST AS ZERO
    
    return set;
}
void list_FREE (data_t set) {
    list_FREE_R(set->head);
}
void list_FREE_R (LINK x) {
    
    //TERMINAL CONDITION
    if (x == NULL)
        return;
    
    //MOVING TO THE END OF THE LIST
    list_FREE_R(x->next);
    
    //FREEING STOCK AND NODE RECURSIVELY
    ITEM_FREE(x->stock);
    free(x);
}

//Input and output
void file_READ (data_t *set, char *filename) {
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    //READING NUMBER OF STOCKS IN THE FILE
    int N_FILE_stocks;
    fscanf(fp, "%d\n", &N_FILE_stocks);
    
    //READING EVERY STOCK INFORMATION AND STORING DATA IN MEMORY
    int N_exchanges;
    char code[MAX_CODE_LENGTH];
    LINK x;
    for (int i = 0; i < N_FILE_stocks; i++) {
        fscanf(fp, "%s %d\n", code, &N_exchanges);
        
        //SEARCH STOCK IN THE LIST BY CODE
        x = stock_SEARCH (*set, code);
        
        //CREATING A NEW STOCK IN THE LIST OR UPLOADING THE EXISTING ONE (stock_READ SWITCHES BETWEEN OPERATIONS)
        if (x == NULL) {
            x = stock_NEW (set, code);
            stock_READ(fp, x, N_exchanges);
        }
        else
            stock_READ(fp, x, N_exchanges);
    }
    
    return;
}
void list_SHOW (data_t *set) {
    fprintf(stdout, "\n\nTHIS IS THE WHOLE LIST OF STOCK:\n\n");
    LINK x;
    for (x = (*set)->head; x != NULL; x = x->next) {
        fprintf(stdout, "\t%s\n", KEYget(x->stock));
        ITEM_SHOW (x->stock);
    }
    
    printf("\n\n");
}

//List operations
void stock_READ (FILE *fp, LINK x, int N_exchanges) {
    ITEM_READ(fp, x->stock, N_exchanges);
}
void stock_SHOW (LINK x) {
    ITEM_SHOW(x->stock);
}
LINK stock_NEW (data_t *set, char *code) {
    
    //HEAD INSERTION
    if ((*set)->head == NULL || KEYcompare(KEYget((*set)->head->stock), code) == 2) {
        (*set)->head = newNode(code, (*set)->head);
        ((*set)->N_stocks)++;
        return (*set)->head;
    }
    
    //SEARCHING RIGHT POSITION
    LINK x, p;
    for (x = (*set)->head->next, p = (*set)->head; x != NULL && KEYcompare(KEYget(x->stock), code) == 1; p = x, x = x->next);
    
    //INSERTION
    p->next = newNode(code, x);
    ((*set)->N_stocks)++;
    return p->next;

}
LINK newNode (char *code, LINK next) {
    
    //DYNAMIC ALLOCATION FOR A NEW NODE
    LINK node = (LINK) malloc(sizeof(node_t));
    if (node == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING VALUES
    node->stock = ITEM_INIT(code);
    node->next = next;
    
    return node;
}

LINK stock_SEARCH (data_t set, char *code) {
    LINK x;
    for (x = set->head; x != NULL; x = x->next) {
        if (KEYcompare(KEYget(x->stock), code) == 0)
            return x;
    }
    
    return NULL;
}
void date_SEARCH_bySTOCK (LINK x, date_t date) {
    date_SEARCH_byITEM(x->stock, date);
}

//Interval search
void maxmin_SEARCH (LINK x, date_t date_A, date_t date_B) {
    maxmin_SEARCH_byITEM(x->stock, date_A, date_B);
}

//Balance
float balanceValue_COMPUTE(LINK x) {
    return balanceValue_COMPUTE_byITEM(x->stock);
}
void balance (LINK x) {
    balance_byITEM(x->stock);
}
