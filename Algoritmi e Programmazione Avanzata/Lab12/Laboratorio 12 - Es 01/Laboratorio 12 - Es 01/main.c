//
//  main.c
//  Laboratorio 12 - Es 01
//
//  Created by Davide Arcolini on 19/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

#define MAX_FILENAME_LENGTH 30

int main(int argc, const char *argv[]) {
    
    //CHECKING TERMINAL ARGUMENTS
    if (argc != 1) {
        fprintf(stderr, "ERROR: insert only program name.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING FILE NAME
    char filename[MAX_FILENAME_LENGTH];
    fprintf(stdout, "- INSERT FILE NAME: ");
    fscanf(stdin, "%s", filename);
    fprintf(stdout, "\n\n");
    
    //READING GRAPH FROM FILE AND STORING DATA IN MEMORY
    graph_t graph = graph_LOAD (filename);
    
    //SHOWING GRAPH
    graph_SHOW(graph);
    
    
    /* --- APPLYING DFS --- */
    fprintf(stdout, "- THIS IS WHAT DFS PRODUCED:\n\n");
    graph_DFS(graph);
    
    
    /* --- FIND MINIMUM SET OF EDGES THAT HAS TO BE REMOVED IN ORDER TO MOVE FROM THE ORIGINAL GRAPH TO A DAG --- */
    int minimum_set_cardinality = 0;
    edge_t *edges = edgeSet_GraphToDAG_SEARCH(graph, &minimum_set_cardinality);
    
    
    
    /* --- FIND IN THE SET OF EDGES GENERATED EARLIER, THE MAXIMUM WEIGHTED SET AND BUILD THE DAG --- */
    
    if (edges != NULL) {
        
        int weight = 0;
        fprintf(stdout, "\n[EDGES REMOVED WITH A TOTAL WEIGHT OF: ");
        for (int i = 0; i < minimum_set_cardinality; i++) {
            weight = weight + edges[i].weight;
            graph_REMOVE(graph, edges[i]);
        }
        fprintf(stdout, "%d].\n\n", weight);
        for (int i = 0; i < minimum_set_cardinality; i++)
            fprintf(stdout, "\t[%s %s - %d] ", edges[i].vertex_START, edges[i].vertex_END, edges[i].weight);
        fprintf(stdout, "\n\n[NEW GRAPH]:\n");
        graph_SHOW(graph);
        
    }
    
    /* --- FIND LONGEST PATH FROM EACH SOURCE TO ALL OTHER VERTEX IN THE DAG --- */
    graph_longestPath_SEARCH(graph);
    
    //FREE
    graph_FREE(graph);
    
    exit(EXIT_SUCCESS);
}
