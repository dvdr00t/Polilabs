//
//  graph.h
//  Laboratorio 12 - Es 01
//
//  Created by Davide Arcolini on 19/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "symbol_table.h"

/* --- DATA --- */         
typedef struct graph_s *graph_t;    //FIRST CLASS ADT USED TO STORE GRAPH INFORMATION

//Edge
typedef struct {
    char vertex_START[MAX_NAME_LENGTH];
    char vertex_END[MAX_NAME_LENGTH];
    int weight;
} edge_t;


/* --- PUBLIC FUNCTION --- */
//Free
void graph_FREE (graph_t graph);

//Input and output
graph_t graph_LOAD (char *filename);    //Load graph in memory from FILE
void graph_SHOW (graph_t graph);        //Show a graph in the stdout

//Operations
void graph_REMOVE (graph_t graph, edge_t edges);

//Visiting graph functions
void graph_DFS (graph_t graph);  //Depth-First visit of the graph

//Find minimum set of edges in the original graph that can be removed to form a DAG
edge_t* edgeSet_GraphToDAG_SEARCH (graph_t graph, int *minimum_set_cardinality);

//Find longest path from each source to every other vertex in the graph
void graph_longestPath_SEARCH (graph_t graph);


#endif /* GRAPH_H */
