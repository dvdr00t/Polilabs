//
//  st.h
//  Laboratorio 10 - Es 04
//
//  Created by Davide Arcolini on 04/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING_LENGTH 30

/* --- DATA --- */

typedef struct symbol_table_t* st;      //Symbol table /storing the size and an array ov "informations"
typedef struct vertex_t* information;   //Information about a vertex (name and net)

typedef int KEY;

/* --- FUNCTIONS --- */

//Create and free
st st_INIT (void);  //Allocation of memory
void st_FREE (st *table);   //Free symbol table

//Input and output
int st_SEARCH (st table, char *name);   //Given a name, seeks it in the data and return its index (-1 if it has not been found)
st st_INSERT (st table, char *name, int net);   //Add information about a vertex to the symbol table
char* st_GET (st table, int index); //Given an index, return the associated name of the vertex
int net_GET (st table, int index);  //Given an index, return the associated net of the vertex
void st_PRINT (st table);  //Show to the user the symbol table
void st_COPY (st *table_DST, st table_SRC); //Copy two table

//Key operation
KEY key_GET (char *string); //Given a net string ("NetXX" where XX is a number value), it returns the XX value

//Sorting operation
int partition (st *table_sorted, int left, int right); //Split the set of data in two subset, one filled with elements that have to be (for some sorting reason) on the left side of the pivot, and one subset with elements that have to be in the right side of the pivot, and returning the new position for the pivot
void vertices_SORT (st *table_sorted);  //Wrapper function
void quickSort (st *table_sorted, int left, int right); //Implemenetation of quicksort algorithm
void swap (st *table_sorted, int i, int j); //Swap two informations
int vertices_COMPARE (information vertex_A, information vertex_B);  //Return 1 if the name of vertex_A comes first in alphabetic order than vertex_B. Else 0

#endif /* ST_H */
