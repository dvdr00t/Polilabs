//
//  pgList.c
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 16/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include "pgList.h"

/* ---- DATA ---- */

//LIST OF USERS
struct nodePg_t {
    pg_t pg;    //User
    link next;  //Link with next user
};

struct pgList_s {
    link head;
    link tail;
    int nPG;
};

/* ---- FUNCTIONS ---- */

//CREATE AND FREE FUNCTIONS
pgList_t pgList_INIT (void) {
    
    //Create an empty node in the list
    pgList_t pgList = NULL;
    
    //Dynamic allocation of space in memory
    pgList = malloc(1 * sizeof(pgList_t));
    if (pgList == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Initializing the list with NULL values
    pgList->head = NULL;
    pgList->tail = NULL;
    pgList->nPG = 0;
    
    return pgList;
}
void pgList_FREE (pgList_t pgList) {
    
    //DEALLOCATING MEMORY UNTIL HEAD POINTER IS NULL
    link x;
    while (pgList->head != NULL) {
        x = pgList->head;   //x stores the head pointer
        pgList->head = pgList->head->next;  //The head pointer becomes the pointer to the next element
        --(pgList->nPG);    //Decreasing number of users
        free(x);    //Deallocating memory
    }
    return;
}

//INPUT AND OUTPUT FUNCTIONS
void pgRead_FILE (pgList_t pgList, char *filename) {
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to open %s FILE.\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    //GETTING DATA FROM FILE
    pg_t user;  //New user
    
    //READ DATA FROM FILE
    while (pg_READ(fp, &user) != EOF) {
        //Initialize user's inventory
        user.equip = equipArrayINIT();
        //Adding user to the list
        pgList_INSERT(pgList, user);
    }
}
void pgList_SHOW (pgList_t pgList, invArray_t invArray) {
    link x;
    
    fprintf(stdout, "\nTHIS IS THE WHOLE LIST OF USERS:\n");
    
    //VISITING ALL NODES OF THE LIST
    for (x = pgList->head; x != NULL; x = x->next) {
        
        //PRINTING USER
        pg_PRINT(stdout, x->pg, invArray);
        printf("\n");
    }
    printf("\n\n");
}
pg_t getUser_INFORMATION (pgList_t pgList) {
    
    //NEW USER
    pg_t user;
    
    //GETTING CODE
    printf("\n\nInsert information for a new user:\n");
    printf("Insert code: ");
    scanf("%s", user.codeUSER);
    
    //CHECKING IF THE USER'S CODE IS ALREADY EXISTING
    if (checkCode(KEYget(user.codeUSER), pgList)){
        printf("Insert name: ");
        scanf("%s", user.nameUSER);
        printf("Insert class: ");
        scanf("%s", user.classUSER);
        
        printf("Insert stats:\n");
        printf("Insert HP: ");
        scanf("%d", &user.stats.hp);
        printf("Insert MP: ");
        scanf("%d", &user.stats.mp);
        printf("Insert ATK: ");
        scanf("%d", &user.stats.atk);
        printf("Insert DEF: ");
        scanf("%d", &user.stats.def);
        printf("Insert MAG: ");
        scanf("%d", &user.stats.mag);
        printf("Insert SPR: ");
        scanf("%d", &user.stats.spr);
        
        //SETTING USER EQUIPMENT
        user.equip = equipArrayINIT();
        
        return user;
    }
    else {
        printf("This user already exists!\n\n");
        strcpy(user.nameUSER, "NOTVALID");
        return user;
    }
}
link searchUSER (pgList_t pgList, char *code) {
    link x;
    
    //SEARCHING USER BY KEY
    for (x = pgList->head; x != NULL; x = x->next)
        if (strcmp(x->pg.codeUSER, code) == 0)
            return x;
    
    return NULL;
}

//ADDING A NEW USER TO THE LIST
KEY KEYget(char *code) {
    KEY k;
    
    //GETTING THE KEY VALUE FROM A CODE
    sscanf(code, "%*c%*c%d", &k);
    return k;
}
int KEYgreater(KEY k1, KEY k2) {
    if (k1 > k2)
        //Returning one if the "int" part of the code for user one comes later than "int" part of the code for user two
        return 1;
    else
        return 0;
}
link newNode (pg_t user, link next) {
    
    //Dynamic allocation of space in memory for a new user
    link nodePg = NULL;
    nodePg = (link) malloc(1 * sizeof(struct nodePg_t));
    if (nodePg == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to save data from FILE in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Assigning information to the new node
    nodePg->pg = user;
    nodePg->next = next;
    
    return nodePg;
}
link uploadTail (pgList_t pgList) {
    link x, p;
    
    //SEARCHING FOR THE LAST NODE OF THE LIST
    for (x = pgList->head, p = NULL; x != NULL; p = x, x = x->next);
    return p;
}
void pgList_INSERT(pgList_t pgList, pg_t user) {
    link x, p;
    KEY k = KEYget(user.codeUSER);
    
    //HEAD INSERTION
    if (pgList->head == NULL || !KEYgreater(k, KEYget(pgList->head->pg.codeUSER))){
        
        //ADDING USER TO THE LIST
        pgList->head = newNode(user, pgList->head); //Adding the user to the list
        pgList->tail = uploadTail(pgList);  //Uploading tail pointer
        (pgList->nPG)++;    //Uploading number of users
        
        return;
    }
    
    //LIST INSERT IN RIGHT POSITION
    for (x = pgList->head->next, p = pgList->head; x != NULL && KEYgreater(k, KEYget(x->pg.codeUSER)); p = x, x = x->next);
    
    p->next = newNode(user, x); //Adding user to the list
    pgList->tail = uploadTail(pgList);  //Uploading tail pointer
    (pgList->nPG)++;    //Uploading number of users
    
}

//CHECKING FUNCTIONS
int checkCode (KEY k, pgList_t pgList) {
    link x;
    
    //SEARCHING FOR A CODE ALREADY EXISTING IN THE LIST
    for (x = pgList->head; x != NULL; x = x->next)
        if (KEYget(x->pg.codeUSER) == k)
            return 0;
    return 1;
}
int checkHeadList (pgList_t pgList) {
    if (pgList->head == NULL)
        return 0;
    return 1;
}

//REMOVING AN EXISTING USER FROM THE LIST
pg_t userACQUISITION (link x) {
    
    //RETURN THE USER
    return x->pg;
}
pg_t* PointerUserACQUISITION (link x) {
    
    //RETURN A POINTER TO THE USER
    return &(x->pg);
}
link pgList_REMOVE (pgList_t pgList, char *code) {
    link x, p;
    
    //DUE TO THE FACT THE LIST IS SORTED, WE CAN STOP SEARCHING FOR THE USER WHEN OUR KEY BECOMES GREATER THAN THE USER CODE KEY IN THE LIST
    for (x = pgList->head, p = NULL; x != NULL && (KEYget(code) >= KEYget(x->pg.codeUSER)); p = x, x = x->next) {
        if (KEYget(x->pg.codeUSER) == KEYget(code)){
            
            //BYPASS IN HEAD
            if (x == pgList->head){
                pgList->head = x->next;
                (pgList->nPG)--;
                return x;
            }
            
            //NORMAL BYPASS
            else {
                p->next = x->next;
                (pgList->nPG)--;
                return x;
            }
        }
    }
    
    return NULL;
}
