//
//  graph.c
//  Laboratorio 10 - Es 04
//
//  Created by Davide Arcolini on 03/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "graph.h"

/* --- DATA --- */

//List structure storing for every vertex its links with other vertices (edges) and edge's weight
struct node {
    int vertex; //Index of a vertex
    int weight; //Weight of the edge
    link edge;  //Edge connection
};

//Data struct for storing informations about a graph
struct graph_t {
    int N_vertex;   //Number of vertices
    int N_edges;    //Number of edges
    int **AdjacencyMatrix;  //Problem solved using adjacency matrix
    link *AdjacencyList;    //Problem solved using adjacency list
};

st table = NULL;    //Global variable storing corrispondance between name of the vertices and their position



/* --- FUNCTIONS --- */

//Input and output
void graph_LOAD (data *graph, char *filename) {
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to opne FILE %s", filename);
        exit(EXIT_FAILURE);
    }
    
    /* --- DATA --- */
    char name_A[MAX_STRING_LENGTH];
    char net_A[MAX_STRING_LENGTH];
    char name_B[MAX_STRING_LENGTH];
    char net_B[MAX_STRING_LENGTH];
    int N_edges = 0;
    int N_vertices = 0;
    
    
    //READING FILE FIRST TIME IN ORDER TO GET THE NUMBER OF EDGES AND CREATE THE SYMBOL TABLE
    //Initializing table
    table = st_INIT();
    
    //Reading FILE
    while (fscanf(fp, "%s %s %s %s %*d", name_A, net_A, name_B, net_B) != EOF) {
        ++N_edges;  //Counting edges
        
        //Checking if first vertex has already been inserted in the table
        if (st_SEARCH(table, name_A) == -1){
            table = st_INSERT(table, name_A, key_GET(net_A));    //Insertion in the table
            ++N_vertices;   //Counting vertices
        }
        
        //Checking if second vertex has already been inserted in the table
        if (st_SEARCH(table, name_B) == -1){
            table = st_INSERT(table, name_B, key_GET(net_B));    //Insertion in the table
            ++N_vertices;   //Counting vertices
        }
    }
    rewind(fp); //Rewind file in order to read it a second time
    
    
    //READING FILE SECOND TIME IN ORDER TO CREATE THE ADJACENCY MATRIX
    //Dynamic allocation for graph in memory
    (*graph) = (data) malloc(sizeof(struct graph_t));
    if ((*graph) == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Initiliazing number of edges and vertices
    (*graph)->N_edges = N_edges;
    (*graph)->N_vertex = N_vertices;
    
    //Dynamic allocation for adjacency matrix in memory
    (*graph)->AdjacencyMatrix = (int**) malloc((*graph)->N_vertex * sizeof(int*));
    if ((*graph)->AdjacencyMatrix == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (*graph)->N_vertex; i++) {
        (*graph)->AdjacencyMatrix[i] = (int*) calloc((*graph)->N_vertex, sizeof(int));
        if ((*graph)->AdjacencyMatrix[i] == NULL) {
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //Filling adjacency matrix
    int weight;
    for (int i = 0; i < (*graph)->N_edges; i++) {
        fscanf(fp, "%s %*s %s %*s %d", name_A, name_B, &weight);
        (*graph)->AdjacencyMatrix[st_SEARCH(table, name_A)][st_SEARCH(table, name_B)] = weight;
        (*graph)->AdjacencyMatrix[st_SEARCH(table, name_B)][st_SEARCH(table, name_A)] = weight;
    }
    
    fclose(fp);
}
void graph_FREE (data *graph) {
    
    //FREEING ADJACENCY LIST AND AJACENCY MATRIX
    for (int i = 0; i < (*graph)->N_vertex; i++) {
        free((*graph)->AdjacencyMatrix[i]);
        if ((*graph)->AdjacencyList[i] != NULL)
            free((*graph)->AdjacencyList[i]);
    }
    free((*graph)->AdjacencyMatrix);
    
    //FREEING SYMBOL TABLE
    st_FREE(&table);
    
}
void graph_SHOW (data graph) {
    printf("This is the GRAPH stored in memory:\n\n");
    
    //SHOWING GRAPH BY ITS AJACENCY MATRIX
    printf("[ADJACENCY MATRIX]\n");
    for (int i = 0; i < graph->N_vertex; i++){
        for (int j = 0; j < graph->N_vertex; j++)
            printf("%d\t", graph->AdjacencyMatrix[i][j]);
        printf("\n");
    }
    printf("\n\n");
    
    //IF IT EXISTS, SHOWING GRAPH BY ITS ADJACENCY LIST
    if (graph->AdjacencyList != NULL) {
        link tmp;
        printf("[ADJACENCY LIST]\n");
        
        for (int v = 0; v < graph->N_vertex; v++){
            tmp = graph->AdjacencyList[v];
            
            printf("Adjacency list of vertex %s\n", st_GET(table, v));
            
            //While tmp != NULL
            while (tmp){
                printf("(%s - Net%d)/%d -> ", st_GET(table, tmp->vertex), net_GET(table, tmp->vertex), tmp->weight);
                tmp = tmp->edge;
            }
            printf("NULL \n\n");
        }
    }
        
}

//Adjacency List operation
void adjList_LOAD(data *graph) {
    
    //DYNAMIC ALLOCATING SPACE IN MEMORY FOR ADJACENCY LIST
    (*graph)->AdjacencyList = (link*) malloc((*graph)->N_vertex * sizeof(link));
    if ((*graph)->AdjacencyList == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING ADJACENCY LIST
    for (int i = 0; i < (*graph)->N_vertex; i++)
        (*graph)->AdjacencyList[i] = NULL;
    
    //READING ADJACENCY MATRIX FROM GRAPH AND GENERATING ADJACENCY LIST
    int start, end, weight;
    for (start = 0; start < (*graph)->N_vertex; start++)
        for (end = start+1; end < (*graph)->N_vertex; end++)
            if ((*graph)->AdjacencyMatrix[start][end] != 0) {
                weight = (*graph)->AdjacencyMatrix[start][end];
                addEdge(graph, start, end, weight);
            }
    
}
void addEdge(data *graph, int start, int end, int weight) {
    link newEdge;
    
    //Adding edge from start to end
    newEdge = createNode(end, weight);
    newEdge->edge = (*graph)->AdjacencyList[start];
    (*graph)->AdjacencyList[start] = newEdge;
    
    //Adding edge from end to start
    newEdge = createNode(start, weight);
    newEdge->edge = (*graph)->AdjacencyList[end];
    (*graph)->AdjacencyList[end] = newEdge;
    
}
link createNode(int vertex, int weight) {
    
    //Dynamic allocating space in memory for a new node of the graph
    link newNode;
    newNode = (link) malloc(sizeof(struct node));
    if (newNode == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to create a new node in memory for graph.\n\n");
        exit(EXIT_FAILURE);
    }
    
    newNode->vertex = vertex;   //Creating a vertex
    newNode->weight = weight;   //Adding weight to the front-end connection
    newNode->edge = NULL;       //Still having no links with other vertices. Those will be created by the function "addEdge"
    
    return newNode; //Returning the new node created
}

//Sorting operation
void graphSorted_SHOW (data graph) {
    
    //COPING THE SYMBOL TABLE IN A MATRIX ON ORDER TO CHANGE ORDER OF ELEMENTS AND PRINT DATA
    st table_sorted = NULL;
    table_sorted = st_INIT();
    st_COPY (&table_sorted, table);
    
    //SORTING VERTICES COPIED 
    vertices_SORT (&table_sorted);
    
    //SHOWING RESULTS
    verticesSorted_PRINT (table_sorted, table, graph);
    
    //FREEING OBJECTS
    st_FREE(&table_sorted);
    
}
void verticesSorted_PRINT (st table_sorted, st table, data graph) {
    
    for (int i = 0; i < graph->N_vertex; i++) {
        printf("Vertex %d: %s - Net%d\n", i+1, st_GET(table_sorted, i), net_GET(table_sorted, i));
        printf("\tConnected vertices: \n");
        connectedVerticesSorted_PRINT (st_GET(table_sorted, i), table, graph);
    }
}
void connectedVerticesSorted_PRINT (char *name, st table, data graph) {
    st table_TMP;
    table_TMP = st_INIT();
    
    //COUNTING HOW MANY EDGES ARE CONNECTED TO THE VERTEX AND INSERTING THEM IN A TABLE
    int index = st_SEARCH(table, name);
    for (int i = 0; i < graph->N_vertex; i++)
        if (graph->AdjacencyMatrix[index][i] != 0)
            st_INSERT(table_TMP, st_GET(table, i), net_GET(table, i));
    
    //SORTING TABLE
    vertices_SORT(&table_TMP);
    
    //SHOWING RESULTS
    st_PRINT(table_TMP);
    
    //FREEING OBJECTS
    st_FREE(&table_TMP);
    
}

//Getting information
int nEdges_GET (data graph) {
    return graph->N_edges;
}
int nVertices_GET (data graph) {
    return graph->N_vertex;
}
int matrixValue_GET (data graph, int vertex_one, int vertex_two) {
    return graph->AdjacencyMatrix[vertex_one][vertex_two];
}
int graph_SEARCH (char *name) {
    return st_SEARCH(table, name);
}
int undergraph_CHECK (data graph, char *name_A, char *name_B, char *name_C) {
    
    /*
    //CHECKING USING THE ADJACENCY MATRIX
    int count = 0;
    if (graph->AdjacencyMatrix[st_SEARCH(table, name_A)][st_SEARCH(table, name_B)] != 0)
        ++count;
    if (graph->AdjacencyMatrix[st_SEARCH(table, name_B)][st_SEARCH(table, name_C)] != 0)
        ++count;
    if (graph->AdjacencyMatrix[st_SEARCH(table, name_C)][st_SEARCH(table, name_A)] != 0)
        ++count;
    
    if (count == 3)
        return 1;
    else
     return 0;
    
     */
     
    //CHECKING USING ADJACENCY LIST
    link x;
    
    //Checking first vertex
    int count = 0;
    for (x = graph->AdjacencyList[st_SEARCH(table, name_A)]; x != NULL; x = x->edge)
        if (x->vertex == st_SEARCH(table, name_B) || x->vertex == st_SEARCH(table, name_C))
            ++count;
    if (count != 2)
        return 0;
    
    //Checking second vertex
    count = 0;
    for (x = graph->AdjacencyList[st_SEARCH(table, name_B)]; x != NULL; x = x->edge)
        if (x->vertex == st_SEARCH(table, name_A) || x->vertex == st_SEARCH(table, name_C))
            ++count;
    if (count != 2)
        return 0;
    
    //Checking third vertex
    count = 0;
    for (x = graph->AdjacencyList[st_SEARCH(table, name_B)]; x != NULL; x = x->edge)
        if (x->vertex == st_SEARCH(table, name_A) || x->vertex == st_SEARCH(table, name_C))
            ++count;
    if (count != 2)
        return 0;
    
    
    return 1;
     
}
