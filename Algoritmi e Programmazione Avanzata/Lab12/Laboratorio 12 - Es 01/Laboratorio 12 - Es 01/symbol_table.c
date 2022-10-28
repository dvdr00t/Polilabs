//
//  symbol_table.c
//  Laboratorio 12 - Es 01
//
//  Created by Davide Arcolini on 19/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "symbol_table.h"

/* --- DATA --- */
typedef char* ITEM;
struct ST_s {
    ITEM *names;
    int N_keys;
    int dimension;
};

/* --- PRIVATE FUNCTION --- */
int STSize_SET (int N_element) {
    int prime = 0;
    int i = 0;
    
    //SERACHING THE SMALLEST PRIME NUMBER GREATER THAN NUMBER OF KEYS
    while (prime < N_element) {
        prime = (6 * i) - 1;
        
        if (prime > N_element)
            return prime;
        
        prime = (6 * i) + 1;
        
        i++;
    }
    
    return prime;
}
static int F1_HASH (ITEM key, int dimension) {
    int hash;
    int base = 127;
    
    //MODULAR METHOD TO FIND AN ALMOST UNIFORM DISTRIBUITION OF KEYS (COLLISION PROBABILITY: 1/M ca)
    for (hash = 0; *key != '\0'; key++)
        hash = (base * hash + *key) % dimension;
    
    return hash;
}
static int F2_HASH (ITEM key, int dimension) {
    int hash;
    int base = 127;
    
    //MODULAR METHOD TO FIND AN ALMOST UNIFORM DISTRIBUITION OF KEYS (COLLISION PROBABILITY: 1/M ca)
    for (hash = 0; *key != '\0'; key++)
        hash = (base * hash + *key) % dimension;
    
    //AVOIDING ZERO RETURN
    hash = (hash % 97) + 1;
    
    return hash;
}
static int full_CHECK (ST hash_table, int index) {
    if (strcmp(hash_table->names[index], "EMPTY") == 0)
        return 1;
    return 0;
}


/* --- PUBLIC FUNCTION --- */

//Create and free
ST ST_INIT (int N_element) {
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY FOR HASH TABLE
    ST hash_table = (ST) malloc (sizeof(struct ST_s));
    if (hash_table == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    hash_table->dimension = STSize_SET(N_element);
    hash_table->N_keys = 0;
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY FOR HASH TABLE NAMES
    hash_table->names = (ITEM*) malloc(hash_table->dimension * sizeof(ITEM));
    if (hash_table->names == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < hash_table->dimension; i++) {
        hash_table->names[i] = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
        if (hash_table->names[i] == NULL) {
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
        
        //INITIALIZING NAMES
        strcpy(hash_table->names[i], "EMPTY");
    }
    
    return hash_table;
}
void ST_FREE (ST hash_table) {
    
    //FREE NAMES
    for (int i = 0; i < hash_table->dimension; i++)
        free(hash_table->names[i]);
    free(hash_table->names);
    
    //FREE HASH TABLE
    free(hash_table);
    
    return;
}

//Input and output
void ST_INSERT (ST hash_table, char *name) {
    
    //COMPUTING FIRST HASH VALUE
    int i = F1_HASH(name, hash_table->dimension);
    int j = F2_HASH(name, hash_table->dimension);
    
    //DOUBLE HASHING
    while (!full_CHECK(hash_table, i))
        i = (i + j) % (hash_table->dimension);
    
    //INSERTING ELEMENT
    strcpy(hash_table->names[i], name);
    (hash_table->N_keys)++;
        
    return;
}
int ST_SEARCH (ST hash_table, char *name) {
    
    //SEARCH WITH LINEAR TIME IN ORDER TO CREATE A CORRESPONDENCE BETWEEN INDICES IN THE HASH TABLE AND INDICES OF ADJACENCY MATRIX
    int index = 0;
    for (int i = 0; i < hash_table->dimension; i++)
        if (strcmp(hash_table->names[i], "EMPTY") != 0) {
            if (strcmp(hash_table->names[i], name) == 0)
                return index;
            ++index;
        }
    return -1;
    
    /*
    //PREVIOUS RESEARCH WAS NORMALLY DONE BUT THE INDEX RETURNED WAS THE INDEX OF THE VALUE STORED IN THE SYMBOL TABLE (WHICH IS NOT NECESSARLY THE INDEX OF THE VALUE IN THE ADJACENCY MATRIX)
    int i = F1_HASH(name, hash_table->dimension);
    int j = F2_HASH(name, hash_table->dimension);
    int count = 0;
    
    while (!full_CHECK(hash_table, i)) {
        if (strcmp(name, hash_table->names[i]) == 0)
            return i;
        else {
            i = (i + j) % hash_table->dimension;
            count++;
        }
    }
 
    return -1;
     
     */
}
char* ST_SEARCH_byIndex (ST hash_table, int index) {
    
    //SEARCH WITH LINEAR TIME IN ORDER TO CREATE A CORRESPONDENCE BETWEEN INDICES IN THE HASH TABLE AND INDICES OF ADJACENCY MATRIX
    int count = 0;
    for (int i = 0; i < hash_table->dimension; i++) {
        if (strcmp(hash_table->names[i], "EMPTY") != 0)
            ++count;
        
        if (count == index + 1)
            return hash_table->names[i];
    }
    return NULL;
}
void ST_SHOW (ST hash_table) {
    fprintf(stdout, "- THIS IS THE HASH TABLE STORING VERTICES: \n\n");
    
    for (int i = 0; i <hash_table->dimension; i++)
        fprintf(stdout, "\t%d - %s\n", i+1, hash_table->names[i]);
    
    printf("\n\n");
    return;
}

//Operation
void ST_COPY (ST *hash_table_DST, ST hash_table_SRC) {
    
    for (int i = 0; i < hash_table_SRC->dimension; i++)
        strcpy((*hash_table_DST)->names[i], hash_table_SRC->names[i]);
    
    (*hash_table_DST)->dimension = hash_table_SRC->dimension;
    (*hash_table_DST)->N_keys = hash_table_SRC->N_keys;
}
