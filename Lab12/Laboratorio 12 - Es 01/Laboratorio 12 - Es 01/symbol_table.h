//
//  symbol_table.h
//  Laboratorio 12 - Es 01
//
//  Created by Davide Arcolini on 19/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 15

/* --- DATA --- */

typedef struct ST_s *ST;    //FIRST CLASS ADT USED AS A HASH TABLE

/* --- PUBLIC FUNCTION --- */

//Create and free
ST ST_INIT (int N_vertices);
void ST_FREE (ST hash_table);

//Input and output
void ST_INSERT (ST hash_table, char *name);
int ST_SEARCH (ST hash_table, char *name);
char* ST_SEARCH_byIndex (ST hash_table, int index);
void ST_SHOW (ST hash_table);

//Operation
void ST_COPY (ST *hash_table_DST, ST hash_table_SRC);

#endif /* SYMBOL_TABLE_H */
