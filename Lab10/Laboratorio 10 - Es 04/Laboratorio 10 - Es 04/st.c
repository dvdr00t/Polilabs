//
//  st.c
//  Laboratorio 10 - Es 04
//
//  Created by Davide Arcolini on 04/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "st.h"

/* --- DATA --- */

struct vertex_t {
    char name[MAX_STRING_LENGTH];   //Name of the vertex
    int net;                        //Net link of the vertex
};

struct symbol_table_t {
    information vertex;
    int size;
};

/* --- FUNCTIONS --- */

//Create and free
st st_INIT (void) {
    
    st table = NULL;
    
    //Dynamic allocation for table
    table = (st) malloc(1 * sizeof(struct symbol_table_t));
    if (table == NULL) {
        fprintf(stderr, "ERROR: some errors occcurred while trying to store data in memory.");
        exit(EXIT_FAILURE);
    }
    
    //Initializing size of symbol table
    table->size = 0;
    
    //Dynamic allocation for matrix table
    table->vertex = (information) malloc(1 * sizeof(struct vertex_t));
    if (table->vertex == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    return table;
}
void st_FREE (st *table) {
    free(table);
}

//Input and output
int st_SEARCH (st table, char *name) {
    int i;
    
    //Linear search
    for (i = 0; i < table->size; i++)
        if (strcmp(name, table->vertex[i].name) == 0)
            return i;   //FOUND, return i if found
    
    return -1;   //NOT FOUND, return -1 if not found
}
st st_INSERT (st table, char *name, int net) {
    
    //RE-ALLOCATION OF SPACE IN MEMORY
    table->size = table->size + 1;
    table->vertex = realloc(table->vertex, table->size * sizeof(struct vertex_t));
    if (table->vertex == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //ADDING ELEMENT TO THE SYMBOL TABLE
    strcpy(table->vertex[table->size-1].name, name);
    table->vertex[table->size-1].net = net;
    
    return table;
}
char* st_GET (st table, int index) {
    return table->vertex[index].name;
}
int net_GET (st table, int index) {
    return table->vertex[index].net;
}
void st_PRINT (st table) {
    int i;
    for (i = 0; i < table->size; i++)
        printf("\t\t%d: %s - Net%d\n", i+1, table->vertex[i].name, table->vertex[i].net);
}
void st_COPY (st *table_DST, st table_SRC) {
    
    //DYNAMIC ALLOCATION OF SPACE FOR TABLE_DST->information
    (*table_DST)->size = table_SRC->size;
    (*table_DST)->vertex = (information) malloc(table_SRC->size * sizeof(struct vertex_t));
    if ((*table_DST)->vertex == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //COPYING INFORMATIONS
    for (int i = 0; i < table_SRC->size; i++) {
        strcpy((*table_DST)->vertex[i].name, table_SRC->vertex[i].name);
        (*table_DST)->vertex[i].net = table_SRC->vertex[i].net;
    }
}

//Key operation
KEY key_GET (char *string) {
    KEY k;
    sscanf(string, "%*c%*c%*c%d", &k);
    return k;
}

//Sorting operation
void vertices_SORT (st *table_sorted) {
    
    //IMPLEMENTATION OF QUICKSORT ALGORITHM
    int left = 0;
    int right = (*table_sorted)->size-1;
    
    quickSort(table_sorted, left, right);
}
void quickSort (st *table_sorted, int left, int right) {
    int pivot_index;
    
    //TERMINAL CONDITION
    if (right <= left)
        return;
    
    //RECURSIVELY COMPUTING INDEX OF PIVOT AND SORTING LEFT AND RIGHT SUBSET OF ELEMENTS. [NB]: no ricombination
    pivot_index = partition(table_sorted, left, right);
    quickSort(table_sorted, left, pivot_index-1);
    quickSort(table_sorted, pivot_index+1, right);
}
int partition (st *table_sorted, int left, int right) {
    int i = left-1;
    int j = right;
    
    //CHOOSING PIVOT (RIGHT-MOST ELEMENT)
    information pivot = NULL;
    pivot = (information) malloc(1 * sizeof(struct vertex_t));
    if (pivot == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    strcpy(pivot->name, (*table_sorted)->vertex[right].name);
    pivot->net = (*table_sorted)->vertex[right].net;
    
    //LOOP TO SORT ACTIVITIES
    for(; ;){
        
        //MOVING ACTIVITIES TO THE LEFT SIDE OF THE PIVOT
        while (vertices_COMPARE(&(*table_sorted)->vertex[++i], pivot));
        while (!vertices_COMPARE(&(*table_sorted)->vertex[--j], pivot))
            if (j == left)
                break;
        if (i >= j)
            break;
        
        swap(table_sorted, i, j);
    }
    
    swap(table_sorted, i, right);
    
    //RETURNING NEW INDEX OF THE PIVOT
    return i;
}
void swap (st *table_sorted, int i, int j) {
    information tmp = NULL;
    tmp = (information) malloc(1* sizeof(struct vertex_t));
    if (tmp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    memcpy(tmp->name, (*table_sorted)->vertex[i].name, MAX_STRING_LENGTH);
    tmp->net = (*table_sorted)->vertex[i].net;
    
    memcpy((*table_sorted)->vertex[i].name, (*table_sorted)->vertex[j].name, MAX_STRING_LENGTH);
    (*table_sorted)->vertex[i].net = (*table_sorted)->vertex[j].net;
    
    memcpy((*table_sorted)->vertex[j].name, tmp->name, MAX_STRING_LENGTH);
    (*table_sorted)->vertex[j].net = tmp->net;
    
}
int vertices_COMPARE (information vertex_A, information vertex_B) {
    
    //COMPARING VERTICES BY ALPHABETIC ORDER
    if (strcmp(vertex_A->name, vertex_B->name) < 0)
        return 1;
    return 0;
}
