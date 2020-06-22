//
//  main.c
//  Laboratorio 4 - Es 3
//
//  Created by Davide Arcolini on 10/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH_STRING 101

//Metachar is the specified character in the regexp while operation can get values between 0 and 5 where:
//0 stands for: "find the specified character metachar[0]"
//1 stands for: "every character is good"
//2 stand for: "every specified characters in metachar is good"
//3 stands for "every specified characters in metachar isn't good"
//4 stands for: "every lowercase characters is good"
//5 stands for: "every uppercase characters is good"

typedef struct{
    char metachar[53];  //26 lowercase alpha_characters + 26 uppercase alpha-characters + end_of_string
    int operation;
} data;

char* find_regexp(char *src, char *regexp, int *counter);   //Added counter to print the exact number of characters
data get_metachar(char *regexp, int *position); //Get the particular metachar informations from regular expressions
int my_compare(char character_string_input, data c);    //Comparing metachar with character from input string
int main(int argc, const char * argv[]){
    
    //Checking arguments
    if (argc != 1){
        fprintf(stderr, "ERROR: insert only program name.\n\n");
        exit(EXIT_FAILURE);
    }
    
    char string_input[MAX_LENGTH_STRING], regexp[MAX_LENGTH_STRING] = {0};
    char *string_output;
    int counter = 0, i;
    
    //Getting input string
    printf("Insert string: ");
    fgets(string_input, MAX_LENGTH_STRING, stdin);
    
    //Getting regular expression
    printf("Insert regular expression you want to find in the string: ");
    scanf("%s", regexp);
    
    //Returning in string output the pointer to the starting characters of the regular expression in the input string
    string_output = find_regexp(string_input, regexp, &counter);
    
    if (string_output == NULL)
        printf("No regular expression found in the input string.\n\n");
    else {
        printf("Regular expression found: ");
        for (i = 0; i < counter; i++)
            printf("%c", *(string_output+i));   //Printing the exact number of characters as the length of the regular expression
        printf(".\n\n");
    }
    
    exit(EXIT_SUCCESS);
}
char* find_regexp(char *src, char *regexp, int *counter){
    int i, j;
    data c;
    
    for (i = 0; i < strlen(src); i++){  //i counter to access every character of the input string
        for (j = 0; j < strlen(regexp); j++){   //j counter to access every character of the regular expression
            c = get_metachar(regexp, &j);   //Saving in c (struct of char[] and int) the informations (characters and what operation we need to do, i.e., what we need to check)
            
            //Comparing characters from input string with informations stored in c. *counter has been added to allow the increment of position of the input string (src) without losing the original position in every iteration. If the comparison ends up well (character matches the metachar in regular expression) we increment the counter and check for the next metachar in regular expression. If not, we reset the counter and skip to the next position (original position + 1) in the input string
            if ((my_compare(src[i+(*counter)], c)) == 0){
                (*counter) = 0;
                break;
            }
            (*counter)++;
            }
        //If we found the exact number of consecutive matches between character from input string and metachar from regular expression, we can know return the pointer to the first character that matches the regular expression
        if (j == strlen(regexp))
            return ((src + i));
    }
    
    //If we didn't find any good matches, we return NULL
    return NULL;
}
data get_metachar(char *regexp, int *position){
    data c;
    int i;
    
    //If the character in regular expression is an alpha-character, we're looking for that particular alpha-character in the input string, 
    if (isalpha(regexp[*position]) == 1){
        c.metachar[0] = regexp[*position];
        c.operation = 0;
        return c;
    }
    else if (regexp[*position] == '.'){
        c.operation = 1;
        return c;
    }
    else if (regexp[*position] == '!'){
        if (isalpha(regexp[*position + 1]) == 1){
            for (i = 0; isalpha(regexp[*position + 1 + i]) == 1; i++){
                c.metachar[i] = regexp[*position + 1 + i];
            }
            c.operation = 2;
            c.metachar[i + 1] = 0;  //Termination value
            (*position) = (*position) + i + 1;
            return c;
        }
        else if (regexp[*position + 1] == '^'){
            for (i = 0; isalpha(regexp[*position + 2 + i]) == 1; i++){
                c.metachar[i] = regexp[*position + 2 + i];
            }
            c.operation = 3;
            c.metachar[i + 1] = 0;  //Termination value
            (*position) = (*position) + i + 2;
            return c;
        }
    }
    else if (regexp[*position] == '\\'){
        if (regexp[*position + 1] >= 'a' && regexp[*position + 1] <= 'z'){
            c.operation = 4;
            (*position)++;
            return c;
        }
        else if (regexp[*position + 1] >= 'A' && regexp[*position + 1] <= 'Z'){
            c.operation = 5;
            (*position)++;
            return c;
        }
    }
    
    c.operation = -1;
    return c;
}
int my_compare(char character_string_input, data c){
    int i;
    switch (c.operation){
        case 0:
        {
            if (character_string_input == c.metachar[0])
                return 1;
            else return 0;
        }
        case 1:
        {
            if (isalpha(character_string_input) == 1)
                return 1;
            else return 0;
        }
        case 2:
        {
            for (i = 0; c.metachar[i] != 0; i++)
                if (character_string_input == c.metachar[i])
                    return 1;
            return 0;
        }
        case 3:
        {
            for (i = 0; c.metachar[i] != 0; i++)
                if (character_string_input == c.metachar[i])
                    return 0;
            return 1;
        }
        case 4:
        {
            if (character_string_input >= 'a' && character_string_input <= 'z')
                return 1;
            else return 0;
        }
        case 5:
        {
            if (character_string_input >= 'A' && character_string_input <= 'Z')
                return 1;
            else return 0;
        }
    }
    
    return 0;
}
