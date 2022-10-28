//
//  main.c
//  Laboratorio 10 - Es 04
//
//  Created by Davide Arcolini on 03/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"

int operation_GET (data graph); //Switch flow of program based in menu choices. Return value is zero if we still are in the program, 1 if we want to terminate execution
void menu_SHOW (void);  //Show the menu


int main(int argc, const char *argv[]) {
    
    //CHECKING TERMINAL PARAMETERS
    if (argc != 1) {
        fprintf(stderr, "ERROR: insert only program name!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING FILE NAME
    char filename[MAX_STRING_LENGTH];
    fprintf(stdout, "Insert FILE name: ");
    fscanf(stdin, "%s", filename);
    
    
    /* --- DATA --- */
    data graph = NULL;
    int end = 0;
    
    //STORING DATA FROM FILE INTO DATA STRUCTURE
    graph_LOAD(&graph, filename);
    
    //MENU IMPLEMENTATION
    while (!end) {
        menu_SHOW();
        end = operation_GET(graph);
    }
    
    //FREE OBJECTS
    graph_FREE(&graph);
    
    //TERMINATE EXECUTION
    exit(EXIT_SUCCESS);
}


void menu_SHOW (void) {
    printf("--- MENU ---\n\n");
    printf("1 - Show graph by vertices in alphabetic order with connected edges\n");
    printf("2 - Check complete undergraph by giving three vertices\n");
    printf("3 - Show graph by adiacency list\n");
    printf("4 - QUIT\n\n");
}
int operation_GET (data graph) {
    int command;
    printf("Select: ");
    scanf("%d", &command);
    
    switch (command) {
            
            //CASE 1: SHOWING THE VERTICES OF THE GRAPH BY ALPHABETIC ORDER AND PRINT EVERY CONNECTED VERTICES BY ALPHABETIC ORDER
        case 1: {
            printf("Those are the vertices sorted by alphabetic order with connected edges:\n\n");
            graphSorted_SHOW(graph);
            return 0;
        }
            
            //CASE 2: GIVEN THREE VERTICES, IT COUNTS THE EDGES AND CHECK IF THOSE THREE VERTICES FORM A COMPLETE (i.e. every vertex is connected to all other vertices) UNDERGRAPH
        case 2: {
            char name_A[MAX_STRING_LENGTH];
            char name_B[MAX_STRING_LENGTH];
            char name_C[MAX_STRING_LENGTH];
            
            //GETTING FIRST VERTEX
            printf("Insert first vertex name: ");
            scanf("%s", name_A);
            
            //CHECKING INPUT
            if (graph_SEARCH(name_A) == -1) {
                printf("This vertex does not exists!\n\n");
                return 0;
            }
            
            
            //GETTING SECOND VERTEX
            printf("Insert second vertex name: ");
            scanf("%s", name_B);
            
            //CHECKING INPUT
            if (graph_SEARCH(name_B) == -1) {
                printf("This vertex does not exists!\n\n");
                return 0;
            }
            
            
            //GETTING THIRD VERTEX
            printf("Insert third vertex name: ");
            scanf("%s", name_C);
            
            //CHECKING INPUT
            if (graph_SEARCH(name_C) == -1) {
                printf("This vertex does not exists!\n\n");
                return 0;
            }
            
            //CHECKING IF THOSE THREE VERTICES FORM A COMPLETE UNDERGRAPH
            if (undergraph_CHECK(graph, name_A, name_B, name_C))
                printf("Yes! Those three vertices ARE a complete undergraph!\n\n");
            else
                printf("Ops! Those three vertices ARE NOT a complete undergraph!\n\n");
            
            return 0;
        }
            
            //CASE 3: GENERATE ADJACENCY MATRIX AND SHOW IT TO THE USER
        case 3: {
            adjList_LOAD(&graph);
            graph_SHOW(graph);
            return 0;
        }
            
            //QUIT EXECUTION
        case 4: {
            char command_confirm;
            printf("Are you sure you want to quit execution?\n Press 0 if you want to go back to the MENU.\n Press any other button to QUIT EXECUTION.\n\n");
            scanf("%*c%c", &command_confirm);
            if (command_confirm != '0') {
                printf("\n\n...QUITTING EXECUTION...\n\n");
                return 1;
            }
            return 0;
        }
            
            //DEFAULT ERROR
        default: {
            printf("Operation not allowed!\n\n");
            return 0;
        }
    }
    
}
