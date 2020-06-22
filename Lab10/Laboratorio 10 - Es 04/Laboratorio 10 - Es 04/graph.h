//
//  graph.h
//  Laboratorio 10 - Es 04
//
//  Created by Davide Arcolini on 03/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "st.h"


/* --- DATA --- */

typedef struct node *link;  //Struct node used for adjacency list
typedef struct graph_t *data;   //Struct graph_t used for storing information about the graph

/* --- FUNCTIONS --- */

//Input and output
void graph_LOAD (data *graph, char *filename);  //Read data from FILE and store them in graph
void graph_FREE (data *graph);  //Free graph
void graph_SHOW (data graph);   //Show to the user the graph by its adjacency matrix and adjacency list (if it has been generated)

//Adjacency List operation
void adjList_LOAD(data *graph); //Read adjacency matrix from graph and store adjacency list in graph
void addEdge(data *graph, int start, int end, int weight);  //Create the edge between two vertices
link createNode(int vertex, int weight);    //Create a new node for the adjacency list

//Sorting operation
void graphSorted_SHOW (data graph); //Show the graph sorted by alphabetic order for vertices and their edges
void verticesSorted_PRINT (st table_sorted, st table, data graph);  //Print every vertices (sorted)
void connectedVerticesSorted_PRINT (char *name, st table, data graph);  //Print, for every vertices, its connected edges (sorted)

//Getting information
int nEdges_GET (data graph);    //Return the number of edges of a graph
int nVertices_GET (data graph); //Return the number of vertices of a graph
int matrixValue_GET (data graph, int vertex_one, int vertex_two);   //Return the weight of an edge given two vertices (0 if for unconnected vertices)
int graph_SEARCH (char *name);  //Given the name of a vertex, return its index (position in the adjacency matrix)
int undergraph_CHECK (data graph, char *name_A, char *name_B, char *name_C);    //GIven three vertices, checking if they form a complete undergraph

#endif /* GRAPH_H */
