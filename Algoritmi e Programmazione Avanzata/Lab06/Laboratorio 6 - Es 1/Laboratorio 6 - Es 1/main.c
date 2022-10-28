//
//  main.c
//  Laboratorio 6 - Es 2
//
//  Created by Davide Arcolini on 21/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME_LENGHT 20

typedef struct node *link;

//List structure storing for every vertex its links with other vertices (edges)
struct node{
    int vertex;
    link edge;
};

//Data struct for storing informations about a graph
typedef struct {
    int N_vertex;   //Number of vertices
    int N_edges;    //Number of edges
    link *AdjacencyList;    //Problem solved using adjacency lists
    int **AdjacencyMatrix;  //Problem solved using adjacency matrix     // NB: symmetric matrix
} graph;

int checkEdges(graph *grafo, int vertex, int i, int *vertexChosen);  //Check if we can count an edge between to vertices or if it has already been counted
int checkVertexCover (int *vertexChosen, graph *grafo); //Check if a particular set of vertices is a vertex cover for the graph
void printVertexCover (graph *grafo, int *vertexChosen);    //Print the vertices that form the vertex cover
void vertexCover (graph *grafo, int *vertexChosen, int *recursionLevel);    //Powerset of vertices
void printGraph (graph *grafo); //Print the adjacency list and matrix for a particular graph
link createNode (int vertex);   //Create a new node (vertex) in the graph unlinked to other vertices. Caller function is in charge of its linking
void addEdge (graph *grafo, int start, int end);    //Create an edge between two vertices in the graph
graph* readingFILE (char *filename);    //Open FILE and store data into a variable of type: "graph"
void checkingFILE (FILE *fp, char *filename);   //Check if a FILE has been opened well


int main(int argc, const char * argv[]) {
    
    //Checking command line parameters
    if (argc != 2){
        fprintf(stderr, "ERROR: insert only program and file name.\n\n");
        exit(EXIT_SUCCESS);
    }
    
    char filename[MAX_FILENAME_LENGHT];
    strcpy(filename, argv[1]);
    
    graph *grafo = NULL;    //Store data from FILE
    int *vertexChosen = NULL;   //Array which stores every vertex we have chosen deep in the recursion in order to print the vertex cover. 1 means we have chosen that particular vertex, 0 means we have left that particular vertex
    int recursionLevel = 0;
    
    //Acquisition of data from FILE
    grafo = readingFILE(filename);
    
    //Dynamic allocation for array "vertexChosen"
    vertexChosen = (int*) malloc(grafo->N_vertex * sizeof(int));
    if (vertexChosen == NULL){
        fprintf(stderr, "ERROR: some errors occurres while trying to compute vertex cover.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Showing the graph
    printGraph(grafo);
    
    //Recursively computing vertex cover
    printf("All possible VERTEX COVERS are:\n\n");
    vertexCover(grafo, vertexChosen, &recursionLevel);

    exit(EXIT_SUCCESS);
    
}


graph* readingFILE (char *filename){
    //Opening FILE and checking if it has been opened well
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    checkingFILE(fp, filename);
    
    //Creating a pointer to graph to be returned to the main and dynamic allocating space in memory in order to store data
    graph *grafo = NULL;
    grafo = (graph*) malloc(sizeof(graph));
    if (grafo == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to save in memory data from FILE: %s.\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    //Reading number of vertices and nodes from first line of FILE
    fscanf(fp, "%d %d", &grafo->N_vertex, &grafo->N_edges);
    
    //Dynamic allocating space in memory for AdjacencyList
    grafo->AdjacencyList = (link*) malloc(grafo->N_vertex * sizeof(link));
    if (grafo->AdjacencyList == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to save in memory data from FILE: %s.\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    //Dynamic allocating space in memory for AdjacencyMatrix
    grafo->AdjacencyMatrix = (int**) malloc(grafo->N_vertex * sizeof(int*));
    if (grafo->AdjacencyMatrix == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to save in memory data from FILE: %s.\n\n", filename);
        exit(EXIT_FAILURE);
    }
    int i; //Counter
    for (i = 0; i < grafo->N_vertex; i++){
        grafo->AdjacencyMatrix[i] = (int*) calloc(grafo->N_vertex, sizeof(int));
        if (grafo->AdjacencyMatrix[i] == NULL){
            fprintf(stderr, "ERROR: some errors occurred while trying to save in memory data from FILE: %s.\n\n", filename);
            exit(EXIT_FAILURE);
        }
    }
    
    
    //Acquisition of edges from FILE and saving data in AdjacencyList and AdjacencyMatrix
    for (i = 0; i < grafo->N_vertex; i++)
        grafo->AdjacencyList[i] = NULL; //Initializing AdjacencyList
    
    int start;  //Starting vertex for a particular edge
    int end;    //Ending vertex for a particular edge
    for (i = 0; i < grafo->N_edges; i++){
        fscanf(fp, "%d %d", &start, &end);
        addEdge(grafo, start, end); //Filling AdjacencyList
        grafo->AdjacencyMatrix[start][end] = 1; //Filling AdjacencyMatrix
        grafo->AdjacencyMatrix[end][start] = 1; //Filling AdjacencyMatrix
    }
    
    fclose(fp); //Closing FILE
    
    return grafo;
}
void addEdge(graph *grafo, int start, int end){
    link newEdge;
    
    //Adding edge from start to end
    newEdge = createNode(end);
    newEdge->edge = grafo->AdjacencyList[start];
    grafo->AdjacencyList[start] = newEdge;
    
    //Adding edge from end to start
    newEdge = createNode(start);
    newEdge->edge = grafo->AdjacencyList[end];
    grafo->AdjacencyList[end] = newEdge;
    
}
link createNode(int vertex){
    //Dynamic allocating space in memory for a new node of the graph
    link newNode;
    newNode = (link) malloc(sizeof(struct node));
    if (newNode == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to create a new node in memory for graph.\n\n");
        exit(EXIT_FAILURE);
    }
    
    newNode->vertex = vertex;   //Creating a vertex
    newNode->edge = NULL;       //Still having no links with other vertices. Those will be created by the function "addEdge"
    
    return newNode; //Returning the new node created
    
}
void vertexCover(graph *grafo, int *vertexChosen, int *recursionLevel){

    //Terminal condition
    if (*recursionLevel == grafo->N_vertex){
        if (checkVertexCover(vertexChosen, grafo))
            printVertexCover(grafo, vertexChosen);
        return;
    }
    
    vertexChosen[(*recursionLevel)] = 1;
    ++(*recursionLevel);
    vertexCover(grafo, vertexChosen, recursionLevel);
    --(*recursionLevel);
    
    vertexChosen[(*recursionLevel)] = 0;
    ++(*recursionLevel);
    vertexCover(grafo, vertexChosen, recursionLevel);
    --(*recursionLevel);
}
int checkEdges(graph *grafo, int vertex, int i, int *vertexChosen){
    int j;
    
    for (j = 0; j < i; j++) //Moving from zero to the actual vertex in order to see if the particular edge we are evaluating has already been counted
        if (vertexChosen[j] == 1)   //If it has been counted, it means we have choosen a vertex (before the one we are considering in the caller function) that has a link with some other vertex through this edge
            if (vertex == j)    //Vertex is the value we have to check. It is the vertex linked with the vertex considered in the caller function) If it is equal to a vertex we have already considered, it means we have already counted that edge.
                return 0;
    return 1;
}
int checkVertexCover (int *vertexChosen, graph *grafo){
    int i;
    //int j;
    int count = 0;  //Counts the edges
    link x;
    
    //Working on adjacency list
    for (i = 0; i < grafo->N_vertex; i++){  //Visiting the whole array of lists
        if (vertexChosen[i] == 1)   //Restriction to only chosen vertex
            for (x = grafo->AdjacencyList[i]; x != NULL; x = x->edge)   //Moving through the adjacency list for a particular vertex
                if(checkEdges(grafo, x->vertex, i, vertexChosen))    //Countin all the new edges (checkEdges checks if the particular edge we are evaluating in the list has been already counted or not
                    ++count;
        if (count >= grafo->N_edges)
            return 1;   //TRUE value
    }
    return 0;   //FALSE value
    
    //Working od adjacency matrix
    /*
    for (i = 0; i < grafo -> N_vertex; i++)        //Visiting all rows of AdjacencyMatrix
        if (vertexChosen[i] == 1)                 //Restriction to only chosen vertex
            for (j = 0; j < grafo->N_vertex; j++)  //Visiting all columns of a particular row
                if (i == 0 || !(j <= i && vertexChosen[j] == 1))    //Exclusion of edger already counted
                    if (grafo->AdjacencyMatrix[i][j] == 1)  //Counting edges
                            ++count;
    
    if (count >= grafo->N_edges)
        return 1;   //TRUE value
    else
        return 0;   //FALSE value
     */
}
void printVertexCover(graph *grafo, int *vertexChosen){
    int i;
    
    printf("{ ");
    for (i = 0; i < grafo->N_vertex; i++)
        if (vertexChosen[i] == 1)
                printf("%d ", i);
    
    printf("}\n");
}
void checkingFILE (FILE *fp, char *filename){
    if (fp == NULL){
        fprintf(stderr, "ERROR: file %s opening failed.\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
void printGraph(graph *grafo){
    int v;
    
    //Showing graph by its adjacency list
    printf("Graph represented by adjacency list:\n\n");
    for (v = 0; v < grafo->N_vertex; v++){
        struct node* temp = grafo->AdjacencyList[v];
        printf("Adjacency list of vertex %d\n", v);
        while (temp){
            printf("%d -> ", temp->vertex);
            temp = temp->edge;
        }
        printf("NULL \n\n");
    }
    
    //Showing graph by its adjacency matrix
    printf("Graph represented by adjacency matrix:\n\n");
    int i;
    for (i = 0; i < grafo->N_vertex; i++){
        for (v = 0; v < grafo->N_vertex; v++)
            printf("%d ", grafo->AdjacencyMatrix[i][v]);
        printf("\n");
    }
    printf("\n\n");
}
