//
//  main.c
//  Laboratorio 07 - Es 3
//
//  Created by Davide Arcolini on 05/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME_LENGTH 30
#define MAX_STRING_LENGTH 50
#define MAX_INVENTOR_CAPACITY 8


//STATS USER AND WEAPON
typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} stat_t;

//INVENTORY OF WEAPONS
typedef struct {
    char name_wp[MAX_STRING_LENGTH];    //Name of the weapon
    char type_wp[MAX_STRING_LENGTH];    //Type of the weapon
    stat_t stat_wp;                     //Stats for the weapon
} inv_t;

//WRAPPER FOR INVENTORY OF WEAPONS
typedef struct {
    inv_t *arrayInv;    //Pointer to the array of weapons
    int nInv;   //Number of weapons avaiable for every user (read from file)
}tabInv_t;

//USER'S INVENTORY + WEAPON IN USE
typedef struct {
    int index_inUse;    //Index of weapon in weapons array
    int wpN;    //Number of weapons in the user's inventory
    inv_t wpArray[MAX_INVENTOR_CAPACITY];   //Weapons array
} tabEquip_t;

//USER'S GENERAL INFORMATION
typedef struct {
    char code[7];    //User's code identificator
    char name_pg[MAX_STRING_LENGTH];    //User's name
    char class[MAX_STRING_LENGTH];      //User's class
    tabEquip_t *equip;  //Pointer to user's inventory
    stat_t stat_pg;    //User's stats
} pg_t;

//LIST OF USERS
typedef struct nodePg_t* link;
struct nodePg_t {
    pg_t pg;    //User
    link next;  //Link with next user
};

//WRAPPER FOR LIST OF USERS
typedef struct {
    link head; //Pointer to the head of the list of users
    link tail; //Pointer to the tail of the list of users
    int nPg;        //Number of users
} tabPg_t;

//KEY VALUE FOR SORTING USERS
typedef int KEY;

tabPg_t tabPg;  //GLOBAL LIST OF USERS
tabInv_t tabInv;    //GLOBAL LIST OF WEAPONS





//USER OPERATION FUNCTION
pg_t computeStats(link x);
pg_t getUser (void);
KEY getCode (void);
link findUser (KEY k);
void addWeapon (link x, int wp);
void deleteWeapon (link x, int wp);
void uploadWeaponInUse (link x, int wp);
void resetUserInventory (pg_t user);
int checkWeapon (link x, int wp);
int checkUserInventory (link x);
int checkCode (KEY k);

//SHOW FUNCTIONS
void showStats (link x);
void showList (void);
void showInventory (void);
void showUserWeaponInUse (link x);
void showUserInventory (link x);

//KEY FUNCTIONS
int KEYgreater (KEY k1, KEY k2);
KEY KEYget (char *code);

//LIST FUNCTIONS
link uploadTail (void);
link addNode (pg_t newPg);
link newNode (pg_t user, link next);
link deleteNode (KEY k);
int checkHeadList (void);

//FILE FUNCTIONS
void checkFILE (FILE *fp, char *filename);
void readFILE_pg (char *pgFilename);
void readFILE_inv (char *invFilename);

//MENU FUNCTIONS
int getOperation (void);
void printMenu (void);






int main(/*int argc, const char * argv[]*/) {

    /*
    if (argc != 1){
        fprintf(stderr, "ERROR: insert only program name.\n\n");
        exit(EXIT_FAILURE);
    }
     */
    
    int end = 0;
    while (!end){
        printMenu();
        end = getOperation();
    }
    exit(EXIT_SUCCESS);
}





//USER OPERATION FUNCTIONS
pg_t computeStats(link x) {
    pg_t user;
    
    //HP
    user.stat_pg.hp = x->pg.stat_pg.hp + x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.hp;;
    if (user.stat_pg.hp < 0)
        user.stat_pg.hp = 0;
    
    //MP
    user.stat_pg.mp = x->pg.stat_pg.mp + x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.mp;
    if (user.stat_pg.mp < 0)
        user.stat_pg.mp = 0;
    
    //ATK
    user.stat_pg.atk = x->pg.stat_pg.atk + x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.atk;
    if (user.stat_pg.atk < 0)
        user.stat_pg.atk = 0;
    
    //DEF
    user.stat_pg.def = x->pg.stat_pg.def + x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.def;
    if (user.stat_pg.def < 0)
        user.stat_pg.def = 0;
    
    //MAG
    user.stat_pg.mag = x->pg.stat_pg.mag + x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.mag;
    if (user.stat_pg.mag < 0)
        user.stat_pg.mag = 0;
    
    //SPR
    user.stat_pg.spr = x->pg.stat_pg.spr + x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.spr;
    if (user.stat_pg.spr < 0)
        user.stat_pg.spr = 0;
    
    return user;
    
}
pg_t getUser (void) {
    pg_t user;
    
    printf("\n\nInsert information for a new user:\n");
    printf("Insert code: ");
    scanf("%s", user.code);
    if (checkCode(KEYget(user.code))){
        printf("Insert name: ");
        scanf("%s", user.name_pg);
        printf("Insert class: ");
        scanf("%s", user.class);
        
        printf("Insert stats:\n");
        printf("Insert HP: ");
        scanf("%d", &user.stat_pg.hp);
        printf("Insert MP: ");
        scanf("%d", &user.stat_pg.mp);
        printf("Insert ATK: ");
        scanf("%d", &user.stat_pg.atk);
        printf("Insert DEF: ");
        scanf("%d", &user.stat_pg.def);
        printf("Insert MAG: ");
        scanf("%d", &user.stat_pg.mag);
        printf("Insert SPR: ");
        scanf("%d", &user.stat_pg.spr);
        
        return user;
    }
    else {
        printf("This user already exists!\n\n");
        strcpy(user.name_pg, "NOTVALID");
        return user;
    }
    
}
KEY getCode (void) {
    char code[7];
    fscanf(stdin, "%s", code);
    return KEYget(code);
}
link findUser (KEY k) {
    link x;
    
    //SEARCHING FOR A PARTICULAR USER AND RETURNING A POINTER TO ITS POSITION IN THE LIST
    for (x = tabPg.head; KEYget(x->pg.code) != k; x = x->next);
    return x;
}
void addWeapon (link x, int wp) {
    int i;
    
    //Adding weapon to the user's inventory in the first empty slot
    for (i = 0; i < MAX_INVENTOR_CAPACITY; i++)
        if (strcmp(x->pg.equip->wpArray[i].name_wp, "EMPTY") == 0) {
            x->pg.equip->wpArray[i] = tabInv.arrayInv[wp-1];
            (x->pg.equip->wpN)++;
            break;
        }
    
    //First weapon added to user's inventory will be the weapon used by the user until you switch it
    if (x->pg.equip->wpN == 1){
        x->pg.equip->index_inUse = 0;
    }
    
    printf("Operation completed!\n\n");
}
void deleteWeapon (link x, int wp) {
    
    //SIGNING THE SLOT OF INVENTORY WITH "EMPTY" IN ORDER TO RE-USE THAT SLOT
    strcpy(x->pg.equip->wpArray[wp-1].name_wp, "EMPTY");
    --(x->pg.equip->wpN);
    
}
void uploadWeaponInUse (link x, int wp) {
    
    //UPLOADING INDEX OF THE ARRAY OF WEAPON FOR THE USER
    x->pg.equip->index_inUse = wp-1;
    
    printf("Operation completed!\n\n");
}
void resetUserInventory (pg_t user) {
    int i;
    for (i = 0; i < MAX_INVENTOR_CAPACITY; i++){
        strcpy(user.equip->wpArray[i].name_wp, "EMPTY");
        strcpy(user.equip->wpArray[i].type_wp, "EMPTY");
        
        user.equip->wpArray[i].stat_wp.hp = 0;
        user.equip->wpArray[i].stat_wp.mp = 0;
        user.equip->wpArray[i].stat_wp.atk = 0;
        user.equip->wpArray[i].stat_wp.def = 0;
        user.equip->wpArray[i].stat_wp.mag = 0;
        user.equip->wpArray[i].stat_wp.spr = 0;
    }
}
int checkWeapon (link x, int wp) {
    
    //CHECKING LIMITS FOR INVENTORY AND EFFECTIVE PRESENCE OF WEAPON
    if (wp > MAX_INVENTOR_CAPACITY || strcmp(x->pg.equip->wpArray[wp-1].name_wp, "EMPTY") == 0){
        printf("This weapon does not exist!\n\n");
        return 0;
    }
    
    //CHECKING IF WEAPON IS IN USE
    if (x->pg.equip->index_inUse == wp-1) {
        printf("This is the weapon in use by the user, you can't delete it.\n\n");
        return 0;
    }
    
    return 1;
}
int checkUserInventory (link x) {
    
    //CHECKING EMPTINESS
    if (x->pg.equip->wpN == 0)
        return 1;
    
    //CHECKING FULLNESS
    if (x->pg.equip->wpN == MAX_INVENTOR_CAPACITY)
        return 2;
    
    return 0;
}
int checkCode (KEY k) {
    link x;
    
    //SEARCHING FOR A CODE ALREADY EXISTING IN THE LIST
    for (x = tabPg.head; x != NULL; x = x->next)
        if (KEYget(x->pg.code) == k)
            return 0;
    return 1;
}

//SHOW FUNCTIONS
void showStats (link x) {
    pg_t user = computeStats(x);
    
    printf("\nUser's informations:\n");
    showUserWeaponInUse(x);
    printf("%s %s %s %d %d %d %d %d %d\n\n", x->pg.code, x->pg.name_pg, x->pg.class, user.stat_pg.hp, user.stat_pg.mp, user.stat_pg.atk, user.stat_pg.def, user.stat_pg.mag, user.stat_pg.spr);
    
}
void showInventory (void) {
    int i;
    
    //Checking if the inventory is empty or not. If not, printing informations in the standard output
    if (tabInv.nInv == 0)
        printf("The list in empty!\n\n");
    else
        for (i = 0; i < tabInv.nInv; i++)
            printf("%d - %s %s %d %d %d %d %d %d\n", i+1, tabInv.arrayInv[i].name_wp, tabInv.arrayInv[i].type_wp, tabInv.arrayInv[i].stat_wp.hp, tabInv.arrayInv[i].stat_wp.mp, tabInv.arrayInv[i].stat_wp.atk, tabInv.arrayInv[i].stat_wp.def, tabInv.arrayInv[i].stat_wp.mag, tabInv.arrayInv[i].stat_wp.spr);
}
void showUserInventory (link x) {
    int i, k = 1;
    printf("\nThese are the weapons in the user's inventory:\n");
    for (i = 0; i < MAX_INVENTOR_CAPACITY; i++)
        if (strcmp(x->pg.equip->wpArray[i].name_wp, "EMPTY") != 0){
            printf("%d - %s %s %d %d %d %d %d %d\n", k, x->pg.equip->wpArray[i].name_wp, x->pg.equip->wpArray[i].type_wp, x->pg.equip->wpArray[i].stat_wp.hp, x->pg.equip->wpArray[i].stat_wp.mp, x->pg.equip->wpArray[i].stat_wp.atk, x->pg.equip->wpArray[i].stat_wp.def, x->pg.equip->wpArray[i].stat_wp.mag, x->pg.equip->wpArray[i].stat_wp.spr);
            
            ++k;
        }
}
void showList (void) {
    link x;
    int i;
    
    //Printing informations in the standard output
    for (x = tabPg.head, i = 0; i < tabPg.nPg; x = x->next, ++i)
        printf("%s %s %s %d %d %d %d %d %d\n", x->pg.code, x->pg.name_pg, x->pg.class, x->pg.stat_pg.hp, x->pg.stat_pg.mp, x->pg.stat_pg.atk, x->pg.stat_pg.def, x->pg.stat_pg.mag, x->pg.stat_pg.spr);

    printf("\n");
    
}
void showUserWeaponInUse (link x) {
    
    //IF THE USER HAS A WEAPON IN USE
    if (x->pg.equip->wpN > 0){
        printf("\nThis is the weapon used by this user:\n");
        printf("%s %s %d %d %d %d %d %d\n\n", x->pg.equip->wpArray[x->pg.equip->index_inUse].name_wp, x->pg.equip->wpArray[x->pg.equip->index_inUse].type_wp, x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.hp, x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.mp, x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.atk, x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.def, x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.mag, x->pg.equip->wpArray[x->pg.equip->index_inUse].stat_wp.spr);
    }
    else
        printf("No weapon in use!\n\n");
    
}

//KEY FUNCTIONS
KEY KEYget(char *code) {
    KEY k;
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

//LIST FUNCTIONS
link newNode (pg_t user, link next) {
    
    //Dynamic allocation of space in memory for a new user
    link nodePg = NULL;
    nodePg = (link) malloc(sizeof(struct nodePg_t));
    if (nodePg == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to save data from FILE in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Assigning information to the new node
    nodePg->pg = user;
    nodePg->next = next;
    
    return nodePg;
}
link addNode (pg_t user) {
    link x, p;
    KEY k = KEYget(user.code);
    
    //HEAD INSERTION
    if (tabPg.head == NULL || !KEYgreater(k, KEYget(tabPg.head->pg.code))){
        
        //Adding user to the list
        tabPg.head = newNode(user, tabPg.head); //Uploading head pointer
        tabPg.tail = uploadTail();  //Uploading tail pointer
        
        return tabPg.head;
    }
    
    
    //LIST INSERT IN RIGHT POSITION
    for (x = tabPg.head->next, p = tabPg.head; x != NULL && KEYgreater(k, KEYget(x->pg.code)); p = x, x = x->next);
    
    p->next = newNode(user, x); //Adding user to the list
    tabPg.tail = uploadTail();  //Uploading tail pointer
    
    return tabPg.head;
}
link deleteNode (KEY k) {
    link x, p;
    
    //SEARCHING FOR THE RIGHT ELEMENT IN THE LIST
    for (x = tabPg.head, p = NULL; x != NULL; p = x, x = x->next) {
        if (KEYget(x->pg.code) == k) {
            
            //BYPASS IN HEAD
            if (x == tabPg.head){
                tabPg.head = x->next;
                --(tabPg.nPg);
                return x;
            }
            //NORMAL BYPASS
            else {
                p->next = x->next;
                --(tabPg.nPg);
                return x;
            }
        }
    }
    
    return NULL;
}
link uploadTail (void) {
    link x, p;
    
    //SEARCHING FOR THE LAST NODE OF THE LIST
    for (x = tabPg.head, p = NULL; x != NULL; p = x, x = x->next);
    return p;
}
int checkHeadList (void) {
    if (tabPg.head == NULL){
        printf("The list is empty!\n\n");
        return 1;
    }
    return 0;
}

//FILE FUNCTIONS
void readFILE_pg (char *pgFilename) {
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(pgFilename, "r");
    checkFILE(fp, pgFilename);
    
    //GETTING DATA FROM FILE
    pg_t user;
    
    //Setting to zero the number of users if it is the first time we add users in the list
    static int firstTime = 1;
    if (firstTime)
        tabPg.nPg = 0;
    firstTime = 0;
    
    //Reading data from file
    while (fscanf(fp, "%s %s %s %d %d %d %d %d %d", user.code, user.name_pg, user.class, &user.stat_pg.hp, &user.stat_pg.mp, &user.stat_pg.atk, &user.stat_pg.def, &user.stat_pg.mag, &user.stat_pg.spr) != EOF){
        
        //Dynamic allocation of space in memory for setting to void the user's equipment
        user.equip = (tabEquip_t*) malloc(sizeof(tabEquip_t));
        if (user.equip == NULL){
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
        resetUserInventory(user);
        user.equip->wpN = 0;
        
        //ADDING USER TO THE LIST
        tabPg.head = addNode(user); //Uploading the head pointer of the list
        (tabPg.nPg)++;  //Uploading the number of users in the list
    }
    
    printf("Operation completed!\n\n");
    fclose(fp);
}
void readFILE_inv (char *invFilename) {
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(invFilename, "r");
    checkFILE(fp, invFilename);
    
    //Dynamic allocation of space in memory for inventory
    fscanf(fp, "%d", &tabInv.nInv);
    tabInv.arrayInv = (inv_t*) malloc(tabInv.nInv * sizeof(inv_t));
    if (tabInv.arrayInv == NULL){
        fprintf(stderr, "ERROR: some errors occurrred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //STORING DATA FROM FILE
    int i;
    for (i = 0; i < tabInv.nInv; i++)
        fscanf(fp, "%s %s %d %d %d %d %d %d", tabInv.arrayInv[i].name_wp, tabInv.arrayInv[i].type_wp, &tabInv.arrayInv[i].stat_wp.hp, &tabInv.arrayInv[i].stat_wp.mp, &tabInv.arrayInv[i].stat_wp.atk, &tabInv.arrayInv[i].stat_wp.def, &tabInv.arrayInv[i].stat_wp.mag, &tabInv.arrayInv[i].stat_wp.spr);
}
void checkFILE (FILE *fp, char *filename) {
    if (fp == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to open %s FILE.\n\n", filename);
        exit(EXIT_FAILURE);
    }
}

//MENU FUNCTIONS
void printMenu (void) {
    printf("----- MENÙ -----\n");
    printf("1 - Show the whole list of users\n");
    printf("2 - Show the whole list of weapons\n");
    printf("3 - Add users to the list from FILE\n");
    printf("4 - Add inventory from FILE\n");
    printf("5 - Add a new user to the list\n");
    printf("6 - Delete an existing user from the list\n");
    printf("7 - Add a new weapon to user's inventory\n");
    printf("8 - Delete an existing weapon from user's inventory\n");
    printf("9 - See stats about a particular user\n");
    printf("0 - Terminate execution\n\n");
    printf("Select your operation: ");
}
int getOperation (void) {
    int command;
    char confirmCommand;
    fscanf(stdin, "%d", &command);
    
    switch (command) {
        case 1: {
            printf("\nThis is the whole list of user in the database:\n");
            if (!checkHeadList())
                showList();
            break;
        }   //SHOW LIST OF USERS
        case 2: {
            printf("\nThis is the whole list of weapons in the inventory:\n");
            showInventory();
            break;
        }   //SHOW INVENTORY OF USER
        case 3: {
            
            char pgFilename[MAX_FILENAME_LENGTH];
            printf("Insert FILE name: ");
            scanf("%s", pgFilename);
            readFILE_pg(pgFilename);
            break;
        }   //READ FILE USER
        case 4: {
            char invFilename[MAX_FILENAME_LENGTH];
            printf("Insert FILE name: ");
            scanf("%s", invFilename);
            readFILE_inv(invFilename);
            break;
        }   //READ FILE INVENTORY
        case 5: {
            
            //GETTING USER INFORMATION
            pg_t user;
            user = getUser();
            
            //getUser checks if the user is not already existing in the list by its code
            if (strcmp(user.name_pg, "NOTVALID") == 0)
                break;
            
            //If it does not exist yet
            else {
                
                //Dynamic allocation of space in memory for user's equipment
                user.equip = (tabEquip_t*) malloc(sizeof(tabEquip_t));
                if (user.equip == NULL){
                    fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
                    exit(EXIT_FAILURE);
                }
                resetUserInventory(user);
                user.equip->wpN = 0;
                
                //ADDING USER TO THE LIST
                tabPg.head = addNode(user); //Uploading head pointer
                tabPg.tail = uploadTail();  //Uploading tail pointer
                (tabPg.nPg)++;  //Uploading number of user
                printf("\n\n");
                break;
            }
        }   //ADD USER
        case 6: {
            
            //CHECKING IF THE LIST IS EMPTY OR NOT
            
            //THE LIST IS EMPTY
            if (checkHeadList())
                break;
            
            //THE LIST IS NOT EMPTY
            else {
                
                KEY k;  //Used to identificate an user in the list
                char code[7];   //Code of the user we want to delete
                link x; //User deleted is returned in order to show it and confirm elimination
                
                //Getting CODE
                printf("Insert code: ");
                scanf("%s", code);
                k = KEYget(code);
                
                //Returning the user deleted (extracted)
                x = deleteNode(k);
                
                //The user has not been found (delete function returned a NULL pointer)
                if (x == NULL)
                    printf("\nUser not found.\n\n");
                
                //User has been found
                else {
                    printf("\nUser found. Here's its informations:\n");
                    showStats(x);
                    
                    //Deciding if we want to delete the user found or keep it
                    printf("Are you sure you want to delete this user?\n");
                    printf("1) I am sure.\n");
                    printf("0) NO, I dont want to delete it anymore!\n\n");
                    printf("Select your operation: ");
                    scanf("%d", &command);
                    if (command == 1){
                        
                        //Deallocating space in the list
                        free(x);
                        printf("Operation completed!\n\n");
                    }
                    else{
                        
                        //Re-inserting the user in the list
                        tabPg.head = addNode(x->pg);
                        tabPg.tail = uploadTail();
                        printf("The user has not been deleted.\n\n");
                    }
                }
                break;
            }
        }   //DELETE USER
        case 7: {
            
            //SELECTING USER
            printf("Select the user:\n\n");
            showList();
            printf("CODE: ");
            link x = findUser (getCode());
            
            //SELECTING WEAPON
            printf("\nSelect a weapon you want to add to the user's inventory:\n");
            showInventory();
            printf("WEAPON NUMBER: ");
            int wp;
            scanf("%d", &wp);
            
            //CHECKING IF USER'S INVENTORY IS FULL OR NOT
            
            //USER INVENTOR IS FULL
            if (checkUserInventory(x) == 2){
                printf("The user's inventory is full! Do you want to sobstitute this weapon with an existing one?\n");
                printf("1 - YES\n");
                printf("2 - NO\n\n");
                printf("Select: ");
                scanf("%d", &command);
                if (command == 2){
                    printf("Back to the MENU.\n\n");
                    break;
                }
                else if (command == 1) {
                    
                    //SELECTING WEAPON THAT WILL BE SOBSTITUTED
                    printf("Select the weapon you want to delete:\n\n");
                    showUserInventory(x);
                    printf("SELECT: ");
                    int wpDelete;
                    scanf("%d", &wpDelete);
                    
                    //DELETING WEAPON
                    if (checkWeapon(x, wpDelete))
                        deleteWeapon(x, wpDelete);
                    
                    //ADDING PREVIOUS WEAPON
                    addWeapon(x, wp);
                    showUserInventory(x);
                    
                    //CHOOSING TO CHANGE THE WEAPON IN USE
                    showUserWeaponInUse(x);
                    printf("Do you want to change user's weapon in use?\n\n1 - YES\n2 - NO\n\nSELECT: ");
                    int a;
                    scanf("%d", &a);
                    if (a == 1){
                        showUserInventory(x);
                        printf("Choose a new weapon to use from user's inventory.\n");
                        printf("SELECT: ");
                        scanf("%d", &wp);
                        
                        //UPLOADING WEAPON IN USE
                        uploadWeaponInUse(x, wp);
                        showUserWeaponInUse(x);
                    }
                    else
                        printf("Back to the MENU.\n\n");
                    break;
                }
            }
            
            //USER INVENTOR IS NOT FULL
            else {
                
                //ADDING WEAPON
                addWeapon(x, wp);
                showUserInventory(x);
                
                //CHOOSING TO CHANGE THE WEAPON IN USE
                showUserWeaponInUse(x);
                printf("Do you want to change user's weapon in use?\n\n1 - YES\n2 - NO\n\nSELECT: ");
                int a;
                scanf("%d", &a);
                if (a == 1){
                    showUserInventory(x);
                    printf("Choose a new weapon to use from user's inventory.\n");
                    printf("SELECT: ");
                    scanf("%d", &wp);
                    
                    //UPLOADING WEAPON IN USE
                    uploadWeaponInUse(x, wp);
                    showUserWeaponInUse(x);
                }
                else
                    printf("Back to the MENU.\n\n");
            }
            
            break;
        }   //ADD WEAPON
        case 8: {
            
            //SELECTING USER
            printf("Select the user:\n\n");
            showList();
            printf("CODE: ");
            link x = findUser (getCode());
            
            
            if (checkUserInventory(x) == 1) {
                printf("User's inventory is empty! You can't delete any weapon.\n\n");
                break;
            }
            else{
                
                //SELECTING WEAPON
                printf("Select the weapon you want to delete:\n\n");
                showUserInventory(x);
                printf("SELECT: ");
                int wp;
                scanf("%d", &wp);
                
                //DELETING WEAPON
                if (checkWeapon(x, wp))
                    deleteWeapon(x, wp);
                
                break;
            }
        }   //DELETE WEAPON
        case 9: {
            
            //SELECTING USER
            printf("Select the user:\n\n");
            if (!checkHeadList()) {
                showList();
                printf("CODE: ");
                link x = findUser (getCode());
                
                //SHOWING STATS
                showStats(x);
            }
            break;
        }   //SHOW STATS
        case 0: {
            printf("Are you sure you want to terminate program execution? Insert 0 if you want to terminate execution or press any other buttom to continue.\n\n");
            fscanf(stdin, "%*c%c", &confirmCommand);
            if (confirmCommand == '0')
                return 1;
            else
                break;
        }   //TERMINTE
            
        default: {
            printf("ERROR: this operation not allowed!\n\n");
            break;
        }   //ERROR
    }
    
    return 0;
}
