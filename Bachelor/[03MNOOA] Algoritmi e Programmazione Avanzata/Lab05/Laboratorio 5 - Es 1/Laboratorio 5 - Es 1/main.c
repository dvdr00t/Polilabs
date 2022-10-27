//
//  main.c
//  Laboratorio 5 - Es 1
//
//  Created by Davide Arcolini on 14/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME_LENGHT 15
#define MAX_STRING_LENGHT 256

//Data about one of the whole friends
typedef struct{
    int songs_number;   //Number of songs selected by a particular friend
    char **songs;       //Names of songs selected by a particular friend
} friend;

//Recursively computing the set of solutions composed of one song for every friend in the set of friends and then printing the particular solution
int printing_results(friend *suggestions, int *choices, int *count, int *friends_number, int pos, int playlist_number);
void reading_file(FILE *fp, friend **suggestions, int *friends_number); //Reading FILE and storing data in an array of struct
void checking_file(FILE *fp, char *filename); //Checking correct opening for a FILE
int main(/*int argc, const char *argv[]*/){
    
    //Checking command line parameters
    /*
    if (argc != 2){
        fprintf(stderr, "ERROR: insert only program and file name.\n\n");
        exit(EXIT_FAILURE);
    }
     */
    char filename[MAX_FILENAME_LENGHT] = "playlist.txt";
    friend *suggestions = NULL; //Array of struct
    int *choices = NULL;    //Keeps the position of the selected song for every friend when we go depth in recursion. So, for example, if choices is |0|0|1|2| when we are in the last instance of the function, it means that we have our set of solutions composed by the first song for the first friend (0 position in **song 2D array), the first song for the second friend, the second song for the third friend and so on...
    int count = 0;  //Counts in which instance of the function we are
    int friends_number = 0; //Read from FILE
    int pos = 0;
    int playlist_number = 0; //Counts how many solutions we have computed
    
    //FILE opening, reading ad storing data in suggestions array of friend type
    FILE *fp;
    fp = fopen(filename, "r");
    checking_file(fp, filename);
    reading_file(fp, &suggestions, &friends_number);
    fclose(fp);
    
    //Dynamic allocation for the array of choices for every friend
    choices = (int*) malloc (friends_number * sizeof(int));
    if (choices == NULL){
        fprintf(stderr, "ERROR: not enough space in memory");
        exit(EXIT_FAILURE);
    }
    
    //Computing and printing the output
    (void) printing_results(suggestions, choices, &count, &friends_number, pos, playlist_number);
    exit(EXIT_SUCCESS);
}
//The idea here is, for every friend, to selected every song and apply for them recursion and move to the next friend. When we are in the last instance of recursion (so last friend), we have made some particular choices before and we can now print all the songs we have choices in combination with all the songs selected by the last friend. Then moving back (backtracking) to second-to-last friend, choosing the next song and use recursion again.
int printing_results(friend *suggestions, int *choices, int *count, int *friends_number, int pos, int playlist_number){
    int i;  //i counter is used, in the last instance of recursion, to move in the 2D array of songs_name (**songs) selected by the last friend
    int j;  //j counter is used to move in the array of choices for every friend
    
    //Terminate condition: we are using the songs selected by the last friend
    if (pos >= (*friends_number) - 1){
        choices[(*count)] = 0;  //Reset the choice made by the last friend to zero
        for (i = 0; i < suggestions[(*count)].songs_number; i++){   //Keeps count of all the songs selected by the last friend
            printf("Playlist number %d:\n", playlist_number + 1);
            
            //Printing the particular solution |choice of friend 1| choice of friend 2| ... | choice of last friend
            for (j = 0; j < (*friends_number); j++)
                printf("%s ", suggestions[j].songs[choices[j]]);
            printf("\n\n");
            choices[(*count)]++;    //Choosing the following song for the last friend
            playlist_number++;      //Counting how many playlists we have done so far
        }
        return playlist_number; //Returning the number of how many playlists we have done so far
    }
    
    choices[(*count)] = 0;  //When we move to the next instance of recursion, we set the choiche made by the particular friend to zero (first song)
    
    //For every song this friend has selected, we apply recursion to the next friend
    for (i = 0; i < suggestions[pos].songs_number; i++){
        (*count)++; //Moving to the next instance
        playlist_number = printing_results(suggestions, choices, count, friends_number, pos + 1, playlist_number);
        (*count)--; //Backtracking
        choices[pos]++; //Moving to the next song for this particular friend
    }
    return playlist_number; //Returning to the main the number of how many playlists we have done so far
    
}
void reading_file(FILE *fp, friend **suggestions, int *friends_number){
    int i, k = 0;
    char song_name_tmp[MAX_STRING_LENGHT];
    
    //Reading first line from FILE and dynamically allocating memory for suggestions array of struct
    fscanf(fp, "%d", friends_number);
    (*suggestions) = (friend*) malloc((*friends_number) * sizeof(friend));
    if ((*suggestions) == NULL){
        fprintf(stderr, "ERROR: not enough space in memory");
        exit(EXIT_FAILURE);
    }
    
    //Reading for every friend how many songs he has selected and storing this data in suggestion.songs_number, the dynamically allocating memory for **song 2D array
    for (i = 0; i < (*friends_number); i++){
        fscanf(fp, "%d", &(*suggestions)[i].songs_number);
        (*suggestions)[i].songs = (char**) malloc((*suggestions)[i].songs_number * sizeof(char*));
        if ((*suggestions)[i].songs == NULL){
            fprintf(stderr, "ERROR: not enough space in memory");
            exit(EXIT_FAILURE);
        }
        
        //Reading every songs name for every friend and storing this data in song_name_tmp, then dynamically allocating memory based on the lenght of song_name_tmp and copying song_name_tmp in the struct field *song[]
        k = 0;
        while (k < (*suggestions)[i].songs_number){
            fscanf(fp, "%s", song_name_tmp);
            (*suggestions)[i].songs[k] = (char*) malloc(strlen(song_name_tmp) * sizeof(char));
            if ((*suggestions)[i].songs[k] == NULL){
                fprintf(stderr, "ERROR: not enough space in memory");
                exit(EXIT_FAILURE);
            }
            strcpy((*suggestions)[i].songs[k], song_name_tmp);
            ++k;
        }
    }
    
}
void checking_file (FILE *fp, char *filename){  //Check if a file is opening well
    if (fp == NULL){
        fprintf(stderr, "ERROR: file %s not found!\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
