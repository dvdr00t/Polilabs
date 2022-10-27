//
//  main.c
//  Laboratorio 6 - Es 1
//
//  Created by Davide Arcolini on 21/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRING_LENGHT 50
#define MAX_FILENAME_LENGHT 15

//First field of the struct node is a new type called item. An element of type "item" stores information about a particular person
typedef struct {
    char code[6];
    char name[MAX_STRING_LENGHT];
    char surname[MAX_STRING_LENGHT];
    char birthday[11];
    char via[MAX_STRING_LENGHT];
    char city[MAX_STRING_LENGHT];
    int cap;
} item;

//Second field of the struct node is apointer to another struct of the same type. An element of type "link" is then a pointer to a struct node
typedef struct node *link;

//Struct definition
struct node{
    item persona;
    link next;
};

//RE-definition of array of chars as KEY.
typedef char* Key;

//New struct to split string of date in int values
typedef struct{
    int year;
    int month;
    int day;
} date;

item ITEMsetvoid(void); //Fill "code" field of item with "ERROR" string. It will be used as an identifier value
link SortListDelete_Dates(link *h, link start, Key k1, Key k2);  //Search for all the items whose KEY (date) comes between two KEYS given as parameters. It returns the link to the struct node containing the item (in order to re-insert that item in the list once extracted) and the head pointer to the list (it may have changed)
link SortListDelete(link *h, Key k); //Search for a particular KEY (code) in the list. It returns the link to the struct node containing the item (in order to re-insert that item in the list once extracted) and the head pointer to the list (it may have changed)
item SortListSearch(link h, Key k); //Search for a particular KEY (code) in the sorted list
item reading_item(void);    //Read informations to fill item fields
Key KEYget(item persona);   //Get the key information (birthday) for a particular item
int KEYequal(Key k1, Key k2);   //Returning 1 (TRUE) if the first KEY (code or date) is equal to the second one, else 0 (FALSE)
int KEYcompare(Key k1, Key k2); //Returning 1 (TRUE) if the first KEY (date) comes earlier in time than the second one, else 0 (FALSE)
link checking_user(link h, item persona);   //Checking if a particular item (user informations) has the same "code" of another one already existing. Returning the link to the struct node containing the item of the user already existing
link newNode(item persona, link next);  //Create a new struct node. It returns the link to that struct node and let the caller function provide its insertion in the list
link listInsSort(link *h, item persona, int *success); //Insert an element in the sorted list in the right place. "int *success" is used as a return value in order to print an output message of FAILURE or SUCCESS
void fprint_list(FILE *fp_print, link h);   //Printing the entire list in a file
void print_persona(item persona);   //Showing (printing in the standard output) the informations about a particular user (item fields)
void print_menu(void);  //Showing the MENU
void reading_file(link *head, char *filename);  //Generating the first list read from FILE
void checking_file(FILE *fp, char *filename);   //Checking if the FILE has been opened well
int main(/*int argc, const char * argv[]*/) {
    
    /*
    //Checking command line parameters
    if (argc != 2){
        fprintf(stderr, "ERROR: insert only program and file name.\n\n");
        exit(EXIT_FAILURE);
    }
     */
    
    char filename[MAX_FILENAME_LENGHT] = "anagrafica.txt", filename_print[MAX_FILENAME_LENGHT], filename_new[MAX_FILENAME_LENGHT];
    char dateOne[11], dateTwo[11];  //KEYS, in order to delete items whose "birthday" field has a value between the KEYS
    char code_key[6];   //KEY used to search an item by its code
    int flag = 1; //While flag is equal to one, keeps the loop going
    int command; //Switch commands
    int success = 0;    //Returning value when a new user has been added or not
    char command_confirm;   //Confirm command used to terminate execution of program
    item persona_found; //Stores information of item found by its code
    link head = NULL; //Creating an empty list
    link x = NULL; //Used as return value for deleting functions
    FILE *fp_print;
    
    reading_file(&head, filename);  //Getting elements and generating the full list sorted by birthdate
    
    while(flag){
        print_menu();
        printf("Select your operation: ");
        scanf("%d", &command);
        
        switch (command){
            case 1:{
                printf("\nA new element will be generated.\n");
                persona_found = reading_item();
                x = listInsSort(&head, persona_found, &success);   //Inserting item in the sorted list
                
                //If the item was not successfully added
                if (success == 0){
                    printf("This is the user with the same code in the list:\n");
                    print_persona(x->persona);
                    
                    printf("Which one do you want to keep:\n");
                    printf("1) The one already existing\n");
                    printf("2) The new one\n\n");
                    printf("Select your operation: ");
                    scanf("%d", &command);
                    
                    if (command == 1){
                        free(x);
                        printf("\nOperation completed.\n\n");
                        break;
                    }
                    else{
                        x = SortListDelete(&head, x->persona.code); //Deleting (without return checking) the user with the same code already existing
                        x = listInsSort(&head, persona_found, &success);   //Adding the new one
                        if (success == 1)
                            printf("\nUser has been added successfully.\n\n");
                        break;
                    }
                }
                else
                    printf("User has been added successfully.\n\n");
                
                break;
            }
            case 2:{
                printf("Insert file name: ");
                scanf("%s", filename_new);
                reading_file(&head, filename_new);
                printf("Operation completed.\n\n");
                break;
            }
            case 3:{
                printf("Insert code: ");
                scanf("%s", code_key);  //Reading code
                
                persona_found = SortListSearch(head, code_key); //Searching a match for the code and returning informations stored in that item
                if (strcmp(persona_found.code, "ERROR") == 0)   //If the item has not been found, its code has been set to "ERROR" in order to be unequivocally recognized
                    printf("\nUser not found.\n\n");
                else{
                    printf("\nUser found. Here's its informations:\n");
                    print_persona(persona_found);   //Print information about the item found by its code
                }
                break;
            }
            case 4:{
                printf("Insert code: ");
                scanf("%s", code_key);  //Reading code
                
                x = SortListDelete(&head, code_key);    //Searching a match for the code, removing the element from the list and returning informations stored in that item
                if (x == NULL)   //If the item has not been found, its code has been set to "ERROR" in order to be unequivocally recognized
                    printf("\nUser not found.\n\n");
                else{
                    printf("\nUser found. Here's its informations:\n");
                    print_persona(x->persona);   //Print information about the item found by its code
                    
                    
                    //Deciding if we want to delete the user found or keep it
                    
                    printf("Are you sure you want to delete this user?\n");
                    printf("1) I am sure.\n");
                    printf("0) NO, I dont want to delete it anymore!\n\n");
                    printf("Select your operation: ");
                    scanf("%d", &command);
                    if (command == 1){
                        free(x);
                        printf("Operation completed!\n\n");
                    }
                    else{
                        head = listInsSort(&head, x->persona, &success);
                        if (success == 1)
                            printf("The user has not been deleted.\n\n");
                        else
                            printf("Some errors occurred.\n\n");
                    }
                }
                break;
            }
            case 5:{
                printf("Insert dates in the format dd/mm/yyyy: ");
                scanf("%s %s", dateOne, dateTwo);   //Reading KEYS
                
                do {
                    x = SortListDelete_Dates(&head, x, dateOne, dateTwo);
                    
                    if (x == NULL)   //If the item has not been found, its code has been set to "ERROR" in order to be unequivocally recognized
                        printf("All operations completed.\n\n");
                    else{
                        printf("\nUser found. Here's its informations:\n");
                        print_persona(x->persona);   //Print information about the item found by its code
                        
                        
                        //DEciding if we want to delete the user found or keep it
                        
                        printf("Are you sure you want to delete this user?\n");
                        printf("1) I am sure.\n");
                        printf("0) NO, I dont want to delete it anymore!\n\n");
                        printf("Select your operation: ");
                        scanf("%d", &command);
                        if (command == 1){
                            free(x);
                            printf("Operation completed!\n\n");
                        }
                        else{
                            head = listInsSort(&head, x->persona, &success);
                            if (success == 1)
                                printf("The user has not been deleted.\n\n");
                            else
                                printf("Some errors occurred.\n\n");
                        }
                    }
                } while (x != NULL);
                break;
            }
            case 6:{
                printf("Insert file name: ");
                scanf("%s", filename_print);    //Reading FILE name
                
                fp_print = fopen(filename_print, "w");  //Creating or opening in "writing" the new FILE
                
                fprint_list(fp_print, head);    //Printing the whole list in the FILE
                
                printf("\nOperation completed.\n\n");
                fclose(fp_print);
                break;
            }
            case 0:{
                printf("You are going to terminate execution of this program. Press 0 to confirm or any other character to continue.\n");
                scanf("%*c%c", &command_confirm);
                if (command_confirm == '0')
                    exit(EXIT_SUCCESS);
                else
                    break;
            }
            default:{
                printf("Operation not allowed!\n\n");
                break;
            }
        }
    }
}


item ITEMsetvoid(void){
    item empty_information;
    strcpy(empty_information.code, "ERROR");    //Fill "code" field of item with "ERROR" string. It will be used as an identifier value
    return empty_information;
}
link SortListDelete_Dates(link *h, link start, Key k1, Key k2){
    link x, p;
    Key date_tmp;

    //Checking if the list is empty or not
    if ((*h) == NULL)
        return NULL;
    
    //If the list is not empty and we are in the first iteration, we initialize x with the head of the list
    if (start == (*h)){
        x = (*h);
        p = NULL;
    }
    //Else (we are not in the first iteration) we initialize x with the start link and p with the previous element
    else
        for (x = (*h), p = NULL; x != start->next; p = x, x = x->next);
    
    //Checking if the user has inserted the first date as the date which comes later in time. If not, just switching.
    if (KEYcompare(k1, k2) == 1){
        date_tmp = k1;
        k1 = k2;
        k2 = date_tmp;
    }
    
    //Searching for the matching items
    for(; x!= NULL; p = x, x = x->next){
        if (KEYcompare(k1, KEYget(x->persona)) == 0 && KEYcompare(k2, KEYget(x->persona)) == 1){
            if (x == (*h)){
                (*h) = x->next; //BYPASS in head
                return x;   //Returning the link to the struct node containing the item that could be deleted
            }
            else{
                p->next = x->next;  //NORMAL BYPASS
                return x;   //Returning the link to the struct node containing the item that could be deleted
            }
        }
    }
    return NULL;
}
link SortListDelete(link *h, Key k){
    link x, p;
    
    //Checking if the list is empty or not
    if ((*h) == NULL)
        return NULL;
    
    //If the list is not empty, searching fot the matching node
    for (x = (*h), p = NULL; x != NULL; p = x, x = x->next)    //VALUTARE L'INSERIMENTO DI UNA FUNZIONE PER L'USCITA ANTICIPATA DAL CICLO
        if (KEYequal(x->persona.code, k)){
            if (x == (*h)){
                (*h) = x->next; //BYPASS in head
                return x;   //Returning the link to the struct node containing the item that could be deleted
            }
            else{
                p->next = x->next;  //NORMAL BYPASS
                return x;   //Returning the link to the struct node containing the item that could be deleted
            }
        }
    return NULL;
            
}
item SortListSearch(link h, Key k){
    link x;
    
    for (x = h; x != NULL; x = x->next)     //VALUTARE L'INSERIMENTO DI UNA FUNZIONE PER L'USCITA ANTICIPATA DAL CICLO
        if (KEYequal(x->persona.code, k))
            return x->persona;
    return ITEMsetvoid();
}
item reading_item(void){
    
    
    //VALUTARE L'INSERIMENTO DI UN CONTROLLO DEI PARAMETRI PASSATI
    item new_persona;
    
    //Reading code
    printf("Insert code: ");
    scanf("%s", new_persona.code);
    
    //Reading name
    printf("Insert name: ");
    scanf("%s", new_persona.name);
    
    //Reading surname
    printf("Insert surname: ");
    scanf("%s", new_persona.surname);
    
    //Reading birthdate
    printf("Insert birthdate: ");
    scanf("%s", new_persona.birthday);
    
    //Reading via
    printf("Insert via: ");
    scanf("%s", new_persona.via);
    
    //Reading city
    printf("Insert city: ");
    scanf("%s", new_persona.city);
    
    //Reading cap
    printf("Insert cap: ");
    scanf("%d", &new_persona.cap);
    
    return new_persona;
    
}
Key KEYget(item persona){
    Key k = malloc(11);
    strcpy(k, persona.birthday);
    return k;
}
int KEYequal(Key k1, Key k2){
    if (strcmp(k1, k2) == 0)
        return 1;
    else
        return 0;
}
int KEYcompare(Key k1, Key k2){
    date birthdayOne, birthdayTwo;
    sscanf(k1, "%d/%d/%d", &birthdayOne.day, &birthdayOne.month, &birthdayOne.year);
    sscanf(k2, "%d/%d/%d", &birthdayTwo.day, &birthdayTwo.month, &birthdayTwo.year);
    
    if (birthdayOne.year < birthdayTwo.year)
        return 1;
    else if (birthdayOne.year > birthdayTwo.year)
        return 0;
    else{
        if (birthdayOne.month < birthdayTwo.month)
            return 1;
        else if (birthdayOne.month > birthdayTwo.month)
            return 0;
        else{
            if (birthdayOne.day < birthdayTwo.day)
                return 1;
            else if (birthdayOne.day > birthdayTwo.day)
                return 0;
        }
    }
    return -1;  //Case in which two dats are equal
}
link checking_user(link h, item persona){
    link x;
    
    for (x = h; x != NULL; x = x->next)
        if (KEYequal(x->persona.code, persona.code) == 1){
            printf("\nThis user already exists!\n");
            return x;   //Returning the link to the struct node containing the duplicate item to the caller function
        }
    return NULL;    //User not found
}
link newNode(item persona, link next){
    link x = malloc(sizeof(*x));    //Dynamic allocation of space for a new node in the list
    if (x == NULL){
        fprintf(stderr, "ERROR: some error occurred while trying to allocate memory.\n\n");
        return NULL;
    }
    //Filling the new node with data
    x->persona = persona;
    x->next = next;
    return x;   //Returning the pointer to the new node added. It will be caller function's business to assign this pointer to a link field in the list
}
link listInsSort(link *h, item persona, int *success){
    link x; //x is used to move through the whole sorted list started from the FIRST ELEMENT (h->next) and moving forward (x = x->next)
    link p; //p is used to move through the whole sorted list started from the HEAD (p = h) and moving forward (p = x). p stores the element before x
    Key k = KEYget(persona);  //k is used to store the birthdate in every iteration as the key, in order to sort the list
    
    //Checking if the user already exists in the list by comparing "code" field of item which determines univocally any users
    x = checking_user(*h, persona);
    if (x != NULL){
        (*success) = 0; //User already exists
        return x;   //Returning the link to the struct node containing the duplicate item in order to print it and let the user decides which one he want to keep
    }

    //HEAD INSERTION
    //If the list is empty (h == NULL) or if the element to insert comes before the first element in the list
    if ((*h) == NULL || KEYcompare(KEYget((*h)->persona), k)){
        (*success) = 1;     //User has been inserted well
        (*h) = newNode(persona, (*h));
        return (*h);
    }

    //NORMAL INSERTION
    //Finding the right position in the list in which the element will be inserted, moving x and p forward in the list
    for (x = (*h)->next, p = (*h); x != NULL && KEYcompare(k, KEYget(x->persona)); p = x, x = x->next);
    p->next = newNode(persona, x);  //When found, adding the new node in the element pointed by p, while x becomes the element pointed by the new element added
    (*success) = 1; //User has been added successfully
    return (*h);   //Returning the new list
}
void fprint_list(FILE *fp_print, link h){
    link x;
    for (x = h; x != NULL; x = x->next)
        fprintf(fp_print, "%s %s %s %s %s %s %d\n", x->persona.code, x->persona.name, x->persona.surname, x->persona.birthday, x->persona.via, x->persona.city, x->persona.cap);
}
void print_persona(item persona){
    printf("Code: %s\n", persona.code);
    printf("Name: %s\n", persona.name);
    printf("Surname: %s\n", persona.surname);
    printf("Birthday: %s\n", persona.birthday);
    printf("Via: %s\n", persona.via);
    printf("City: %s\n", persona.city);
    printf("Cap: %d\n\n", persona.cap);
}
void print_menu(void){
    printf("MENÙ:\n");
    printf("1) Add a new element from KEYBOARD.\n");
    printf("2) Add new elements from FILE.\n");
    printf("3) Find an element by code.\n");
    printf("4) Delete an element.\n");
    printf("5) Delete all the element between two dates.\n");
    printf("6) Save the list in a FILE.\n");
    printf("0) TERMINATE EXECUTION.\n\n");
}
void reading_file(link *head, char *filename){
    FILE *fp;
    fp = fopen(filename, "r");
    checking_file(fp, filename);
    item persona;
    int success = 0;
    
    //Reading data from file
    while (fscanf(fp, "%s %s %s %s %s %s %d", persona.code, persona.name, persona.surname, persona.birthday, persona.via, persona.city, &persona.cap) == 7)
        (*head) = listInsSort(head, persona, &success);    //Inserting data in a sorted list
    fclose(fp);
}
void checking_file(FILE *fp, char *filename){
    if (fp == NULL){
        fprintf(stderr, "ERROR: file %s not found or can not be opened.\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
