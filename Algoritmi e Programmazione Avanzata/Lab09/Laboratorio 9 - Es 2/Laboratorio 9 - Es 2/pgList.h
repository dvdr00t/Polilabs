//
//  pgList.h
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#ifndef PGLIST_H
#define PGLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pg.h"

typedef int KEY;

/* ---- DATA ---- */

//ADT for list of users
typedef struct nodePg_t* link;
typedef struct pgList_s *pgList_t;


/* ----  FUNCTIONS  ----- */

//CREATE AND FREE FUNCTIONS
pgList_t pgList_INIT (void);
void pgList_FREE (pgList_t pgList);

//INPUT AND OUTPUT FUNCTIONS
void pgRead_FILE (pgList_t pgList, char *filename);
void pgList_SHOW (pgList_t pgList, invArray_t invArray);
pg_t getUser_INFORMATION (pgList_t pgList);
link searchUSER (pgList_t pgList, char *code);

//ADDING A NEW USER TO THE LIST
void pgList_INSERT (pgList_t pgList, pg_t user);
link uploadTail (pgList_t pgList);
link newNode (pg_t user, link next);

//KEY FUNCTIONS
KEY KEYget(char *code);
int KEYgreater(KEY k1, KEY k2);

//CHECKING FUNCTIONS
int checkCode (KEY k, pgList_t pgList);
int checkHeadList (pgList_t pgList);

//REMOVING AN EXISTING USER FROM THE LIST
pg_t userACQUISITION (link x);
pg_t* PointerUserACQUISITION (link x);
link pgList_REMOVE (pgList_t pgList, char *code);


#endif /* PGLIST_H */
