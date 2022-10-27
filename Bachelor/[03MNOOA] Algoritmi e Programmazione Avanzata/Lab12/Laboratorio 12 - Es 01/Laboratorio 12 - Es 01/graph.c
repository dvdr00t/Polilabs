//
//  graph.c
//  Laboratorio 12 - Es 01
//
//  Created by Davide Arcolini on 19/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "graph.h"

/* --- PRIVATE DATA --- */

//Node in the adjacency list
typedef struct node *link;
struct node {
    int vertex; //Number of the vertex
    int weight; //Weight of the edge
    link next;  //Link with the next vertex
};


/* --- PUBLIC DATA --- */

//Graph
struct graph_s {
    int N_vertices; //Number of vertices in the graph
    int N_edges;    //Number of edges in the graph
    int **Adjacency_Matrix; //Adjacency matrix
    link *Adjacency_List;   //Adjacency list
    link z; //Sentinel node
    ST hash_table;  //Symbol table
};



/* --- PRIVATE FUNCTION --- */

//GRAPH BASIC OPERATIONS
//Create and free
static link node_NEW (int vertex, int weight, link next) {
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY FOR A NEW NODE
    link x = (link) malloc(sizeof(struct node));
    if (x == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING VARIABLES
    x->vertex = vertex;
    x->weight = weight;
    x->next = next;
    
    return x;
}
static graph_t graph_INIT (int N_vertices) {
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY FOR THE GRAPH
    graph_t graph = (graph_t) malloc(sizeof(struct graph_s));
    if (graph == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING VARIABLES
    graph->N_vertices = N_vertices;
    graph->N_edges = 0;
    graph->z = node_NEW(-1, -1, NULL);  //Sentinel node creation
    graph->hash_table = ST_INIT(N_vertices);    //Hash table initialization
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY FOR ADJACENCY LIST
    graph->Adjacency_List = (link*) malloc(N_vertices * sizeof(link));
    if (graph->Adjacency_List == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N_vertices; i++) {
        graph->Adjacency_List[i] = (link) malloc(sizeof(struct node));
        if (graph->Adjacency_List[i] == NULL) {
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
        //INITIALIZING LIST
        graph->Adjacency_List[i] = graph->z;
    }
    
    //DYNAMIC ALLOCATION IN MEMORY FOR ADJACENCY MATRIX
    graph->Adjacency_Matrix = (int**) malloc(N_vertices * sizeof(int*));
    if (graph->Adjacency_Matrix == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N_vertices; i++) {
        graph->Adjacency_Matrix[i] = (int*) calloc(N_vertices, sizeof(int));
        if (graph->Adjacency_Matrix[i] == NULL) {
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return graph;
}
//Input and output
static void list_SHOW_R (graph_t graph, link x) {
    
    //TERMINAL CONDITION
    if (x == graph->z) {
        fprintf(stdout, "NULL\n");
        return;
    }
    //SHOWING CURRENT NODE
    fprintf(stdout, "[%s / %d] -> ", ST_SEARCH_byIndex(graph->hash_table, x->vertex), x->weight);
    
    //RECURSION
    list_SHOW_R(graph, x->next);
}
//Operation
static void graph_COPY (graph_t *graph_DST, graph_t graph_SRC) {
    
    //COPYING VARIABLE VALUES
    (*graph_DST)->N_edges = graph_SRC->N_edges;
    (*graph_DST)->N_vertices = graph_SRC->N_vertices;
    (*graph_DST)->z = graph_SRC->z;
    
    //COPYING ADJACENCY MATRIX
    for (int i = 0; i < graph_SRC->N_vertices; i++)
        for (int j = 0; j < graph_SRC->N_vertices; j++)
            (*graph_DST)->Adjacency_Matrix[i][j] = graph_SRC->Adjacency_Matrix[i][j];
    
    //COPYING ADJACENCY LIST
    for (int i = 0; i < graph_SRC->N_vertices; i++)
        (*graph_DST)->Adjacency_List[i] = graph_SRC->Adjacency_List[i];
    
    //COPYING SYMBOL TABLE
    ST_COPY(&(*graph_DST)->hash_table, graph_SRC->hash_table);
}

//EDGE OPERATIONS
//Create and free
static edge_t edge_CREATE (graph_t graph, int index_START, int index_END, int weight) {
    
    //CREATING EDGE
    edge_t edge;
    
    //ASSIGNING VALUES
    strcpy(edge.vertex_START, ST_SEARCH_byIndex(graph->hash_table, index_START));
    strcpy(edge.vertex_END, ST_SEARCH_byIndex(graph->hash_table, index_END));
    edge.weight = weight;
    
    
    return edge;
}
//Input and output
static int edge_READ (FILE *fp, edge_t *edge) {
    
    //READ EDGE VALUES
    if (fscanf(fp, "%s %s %d\n", edge->vertex_START, edge->vertex_END, &edge->weight) == 3)
        return 1;
    return 0;
}
//Operations
static edge_t* edge_FILL (graph_t graph) {
    
    //DYNAMIC ALLOCATION FOR SET OF ITEMS (EDGES)
    edge_t *edges = (edge_t*) malloc(graph->N_edges * sizeof(edge_t));
    if (edges == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING EDGES FROM ADJACENCY LIST
    int count = 0;
    link x;
    for (int V_index = 0; V_index < graph->N_vertices; V_index++) {
        for (x = graph->Adjacency_List[V_index]; x != graph->z; x = x->next) {
            strcpy(edges[count].vertex_START, ST_SEARCH_byIndex(graph->hash_table, V_index));
            strcpy(edges[count].vertex_END, ST_SEARCH_byIndex(graph->hash_table, x->vertex));
            edges[count++].weight = x->weight;
        }
    }
    
    return edges;
}
static void edge_INSERT (graph_t graph, int index_START, int index_END, int weight) {
    
    //ADJACENCY LIST INSERT
    link newEdge;
    newEdge = node_NEW(index_END, weight, graph->z);
    newEdge->next = graph->Adjacency_List[index_START];
    graph->Adjacency_List[index_START] = newEdge;
    
    //ADJACENCY MATRIX INSERT
    graph->Adjacency_Matrix[index_START][index_END] = weight;
    
    //COUNTING EDGES
    graph->N_edges++;
}
static void edge_COPY (edge_t *edge_DST, edge_t edge_SRC) {
    
    //COPYING VALUES
    strcpy(edge_DST->vertex_START, edge_SRC.vertex_START);
    strcpy(edge_DST->vertex_END, edge_SRC.vertex_END);
    edge_DST->weight = edge_SRC.weight;
}

//OTHERS
static long double factorial (int N) {
    
    //TERMINAL CONDITION
    if (N == 0)
        return 1;
    
    //RECURSIVELY COMPUTING FACTORIAL
    return N * factorial(N-1);
}
static long double binomialCoefficient (int N, int K) {
    return (factorial(N) / (factorial(K) * factorial(N-K)));
}

//GRAPH ADVANCED OPERATION
//Visit (DFS)
static void graph_DFS_R (graph_t graph, edge_t edge, int *time, int *pre, int *post, int *st) {
    
    //VARIABLES USED
    int index_END = ST_SEARCH(graph->hash_table, edge.vertex_END);
    int index_START;
    edge_t edge_TMP;
    
    //AVOIDING VISITING SELF-LOOP
    if (strcmp(edge.vertex_START, edge.vertex_END) != 0)
        fprintf(stdout, "\t[%s - %s]: TREE EDGE\n", edge.vertex_START, edge.vertex_END);
    
    //ASSIGNIN PARENT TO THE NEW VERTEX DISCOVERED
    st[ST_SEARCH(graph->hash_table, edge.vertex_END)] = ST_SEARCH(graph->hash_table, edge.vertex_START);
    pre[index_END] = (*time)++;
    
    //VISITING THE GRAPH THROUGH ALL EDGES STARTING FROM THIS VERTEX
    for (link x = graph->Adjacency_List[index_END]; x != graph->z; x = x->next) {
        if (pre[x->vertex] == -1)
            graph_DFS_R(graph, edge_CREATE(graph, index_END, x->vertex, 0), time, pre, post, st);
        else {
            index_START = x->vertex;
            edge_TMP = edge_CREATE(graph, index_END, index_START, 0);
            
            //SHOWING RESULTS
            if (post[index_START] == -1)
                fprintf(stdout, "\t[%s - %s]: BACKWARD EDGE\n", edge_TMP.vertex_START, edge_TMP.vertex_END);
            else {
                if (pre[index_START] > pre[index_END])
                    fprintf(stdout, "\t[%s - %s]: FORWARD EDGE\n", edge_TMP.vertex_START, edge_TMP.vertex_END);
                else
                    fprintf(stdout, "\t[%s - %s]: CROSS EDGE\n", edge_TMP.vertex_START, edge_TMP.vertex_END);
            }
        }
    }
    
    //WE HAVE COMPLETED THIS VERTEX IN THE RECURSION
    post[index_END] = (*time)++;
}

//Check (DAG)
static int source_CHECK (graph_t graph, int V_index) {
    
    //VISITING ADJACENCY MATRIX
    for (int i = 0; i < graph->N_vertices; i++)
        if (graph->Adjacency_Matrix[i][V_index] != 0)
            return 0;
    
    return 1;
}
static int graphDAG_CHECK_R (graph_t graph, edge_t edge, int *time, int *pre, int *post) {
    
    //VARIABLES USED
    int index_END = ST_SEARCH(graph->hash_table, edge.vertex_END);
    int index_START;
    edge_t edge_TMP;
    int backEdges_Count = 0;
    
    //VISITING THE GRAPH THROUGH ALL EDGES STARTING FROM THIS VERTEX
    pre[index_END] = (*time)++;
    for (link x = graph->Adjacency_List[index_END]; x != graph->z; x = x->next) {
        if (pre[x->vertex] == -1)
            backEdges_Count = backEdges_Count + graphDAG_CHECK_R(graph, edge_CREATE(graph, index_END, x->vertex, 0), time, pre, post);
        else {
            index_START = x->vertex;
            edge_TMP = edge_CREATE(graph, index_END, index_START, 0);
            
            //BACK-EDGE HAS BEEN FOUND, GRAPH IS NOT A DAG, COUNTING BACK-EDGES
            if (post[index_START] == -1)
                ++backEdges_Count;
            
        }
    }
    
    //WE HAVE COMPLETED THIS VERTEX IN THE RECURSION
    post[index_END] = (*time)++;
    
    return backEdges_Count;
}
static int graphDAG_CHECK (graph_t graph) {
    
    //VARIABLES USED
    int time = 0;   //Time counter
    int *pre = NULL;   //Array storing the time in which the particular vertex has been discovered
    int *post = NULL;  //Array storing the time in which the particular vertex has been completed (we have gone through every edges with the recursion)
    int backEdges_Count = 0;
    
    //DYNAMIC ALLOCATION OS SPACE IN MEMORY FOR ARRAYS
    pre = (int*) malloc(graph->N_vertices * sizeof(int));
    post = (int*) malloc(graph->N_vertices * sizeof(int));
    if (pre == NULL || post == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING ARRAYS
    for (int i = 0; i < graph->N_vertices; i++) {
        pre[i] = -1;
        post[i] = -1;
    }
    
    //STARTING DFS_CHECK (LOOKING FOR BACKEDGES) FROM EVERY VERTEX
    for (int V_index = 0; V_index < graph->N_vertices; V_index++)
        backEdges_Count = backEdges_Count + graphDAG_CHECK_R(graph, edge_CREATE(graph, V_index, V_index, 0), &time, pre, post);
    
    //FREE
    free(pre);
    free(post);
    
    return backEdges_Count;
}
static int solution_CHECK (graph_t graph_TMP, edge_t *edges_TMP, int edge_count) {
    
    //REMOVING EDGES FROM GRAPH
    for (int i = 0; i < edge_count; i++)
        graph_REMOVE (graph_TMP, edges_TMP[i]);
    
    //CHECKING SOLUTION
    int result = graphDAG_CHECK(graph_TMP);
    
    //RE-INSERTING EDGES
    for (int i = 0; i < edge_count; i++)
        edge_INSERT(graph_TMP, ST_SEARCH(graph_TMP->hash_table, edges_TMP[i].vertex_START), ST_SEARCH(graph_TMP->hash_table, edges_TMP[i].vertex_END), edges_TMP[i].weight);
    
    return result;
}
static edge_t** solution_RESET (graph_t graph_TMP, edge_t **edges_ToBeRemoved, int edge_count) {
    
    //COMPUTING NEW BINOMIAL COEFFICIENT (MAXIMUM NUMBER OF VALID SOLUTIONS
    long double dimension = binomialCoefficient(graph_TMP->N_edges, edge_count);
    
    //RE-ALLOCATING SPACE IN MEMORY
    edge_t **edges_ToBeRemoved_NEW = (edge_t**) malloc(dimension * sizeof(edge_t*));
    if (edges_ToBeRemoved_NEW == NULL) {
        fprintf(stdout, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dimension; i++) {
        //FREE OLD
        free(edges_ToBeRemoved[i]);
        
        //NEW SPACE
        edges_ToBeRemoved_NEW[i] = (edge_t*) malloc(edge_count * sizeof(edge_t));
        if (edges_ToBeRemoved_NEW[i] == NULL) {
            fprintf(stdout, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //RESETTING VALUES
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < edge_count; j++) {
            strcpy(edges_ToBeRemoved_NEW[i][j].vertex_START, "VOID");
            strcpy(edges_ToBeRemoved_NEW[i][j].vertex_END, "VOID");
            edges_ToBeRemoved_NEW[i][j].weight = -1;
        }
    
    free(edges_ToBeRemoved);
    
    return edges_ToBeRemoved_NEW;
    
}

//Recursive generate set of edges that can be removed in order to form a DAG
static int edgeSet_GraphToDAG_SEARCH_R (graph_t graph_TMP, edge_t *edges_TMP, edge_t *edges, edge_t **edges_ToBeRemoved, int pos, int *count, int *edge_count, int *N_backEdges, int start) {
    
    //TERMINAL CONDITION
    if (pos <= (*N_backEdges)) {
        
        //CHECKING IF THE SOLUTION IS VALID
        if (solution_CHECK(graph_TMP, edges_TMP, *edge_count) == 0) {
            
            //CHECKING IF THE CARDINALITY OF THIS SOLUTION IS BETTER (LOWER) THAN THE MAXIMUM CARDINALITY SOLUTION EVER FOUND
            if ((*edge_count) < (*N_backEdges)) {
                
                //IF SO, RESETTING SOLUTIONS AND UPLOADING NEW CARSINALITY VALUE
                edges_ToBeRemoved = solution_RESET (graph_TMP, edges_ToBeRemoved, (*edge_count));
                (*N_backEdges) = (*edge_count);
            }
            
            //UPLOADING SOLUTION
            for (int i = 0; i < *edge_count; i++)
                edge_COPY(&edges_ToBeRemoved[*count][i], edges_TMP[i]);
            (*count)++;
        }
    }
    else
        return (*count);
    
    //INSERT AN EDGE IN THE SET OF SOLUTION
    for (int i = start; i < graph_TMP->N_edges; i++) {
        
        //ADDING EDGE TO TMP SOLUTION
        edge_COPY(&edges_TMP[pos], edges[i]);
        (*edge_count)++;
        
        //RECURSION
        edgeSet_GraphToDAG_SEARCH_R(graph_TMP, edges_TMP, edges, edges_ToBeRemoved, pos+1, count, edge_count, N_backEdges, i+1);
        
        //REMOVING EDGE
        (*edge_count)--;
    }
    
    return (*count);
}

//Find longest path from each source to every other vertex in the graph
static void graphDAG_DFS_TS (graph_t graph, int V_index, int *array_TS, int *prev, int *time) {
    
    //INITILAZING VARIABLES
    link x;
    prev[V_index] = 0;
    
    //VISITING EVERY VERTEX IN THE ADJACENCY LIST
    for (x = graph->Adjacency_List[V_index]; x != graph->z; x = x->next)
        
        //IF WE DO NOT HAVE DISCOVER THIS VERTEX YET
        if (prev[x->vertex] == -1)
            
            //APPLYING DFS AGAIN
            graphDAG_DFS_TS(graph, x->vertex, array_TS, prev, time);
    
    //ASSIGNING THE COMPLETED VERTEX IN THE SPECIFIC POSITION (GIVEN BY TIME) IN THE TOPOLOGICAL ORDER
    array_TS[(*time)++] = V_index;
}
static void longestPath_SEARCH_R (graph_t graph, int V_index, int *array_TS_R) {
    
    //DYNAMIC ALLOCATION FOR DISTANCE ARRAY
    int *dst = (int*) malloc(graph->N_vertices * sizeof(int));
    if (dst == NULL) {
        fprintf(stdout, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING ARRAY
    for (int i = 0; i < graph->N_vertices; i++)
        dst[i] = INT_MIN;
    dst[V_index] = 0;
    
    //COMPUTING LONGEST PATH
    //VISITING EVERY VERTEX...
    for (int i = 0; i < graph->N_vertices; i++) {
        
        //...IN TOPOLOGICAL ORDER
        int U_index = array_TS_R[i];
        
        //VISITING EVERY CONNECTED VERTEX
        link x;
        for (x = graph->Adjacency_List[U_index]; x != graph->z; x = x->next) {
            
            //APPLY RELAXATION
            if (dst[x->vertex] < dst[U_index] + graph->Adjacency_Matrix[U_index][x->vertex])
                dst[x->vertex] = dst[U_index] + graph->Adjacency_Matrix[U_index][x->vertex];
        }
    }
    
    
    //SHOWING RESULTS
    for (int i = 0; i < graph->N_vertices; i++)
        if (dst[i] > 0)
            fprintf(stdout, "\t\t[ARRIVING NODE: %s]: %d\n", ST_SEARCH_byIndex(graph->hash_table, i), dst[i]);
    
}


/* --- PUBLIC FUNCTION --- */
//Free
void graph_FREE (graph_t graph) {
    
    //FREE ADJACENCY MATRIX
    for (int i = 0; i < graph->N_vertices; i++)
        free(graph->Adjacency_Matrix[i]);
    free(graph->Adjacency_Matrix);
    
    //FREE ADJACENCY LIST
    link next;
    for (int V_index = 0; V_index < graph->N_vertices; V_index++)
        for (link x = graph->Adjacency_List[V_index]; x != graph->z; x = next) {
            next = x->next;
            free(x);
        }
    free(graph->Adjacency_List);
    free(graph->z);
    
    //FREE SYMBOL TABLE
    ST_FREE(graph->hash_table);
    
    //FREE GRAPH
    free(graph);
    
    return;
}

//Input and output
graph_t graph_LOAD (char *filename) {
    
    //FILE OPENING
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s.\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    //READING NUMBER OF VERTICES FROM FILE
    int N_vertices;
    fscanf(fp, "%d\n", &N_vertices);
    
    //INITIALIZING GRAPH
    graph_t graph = graph_INIT(N_vertices);
    
    //READING VERTICES AND FILLING SYMBOL TABLE
    char vertex[MAX_NAME_LENGTH];
    for (int i = 0; i < N_vertices; i++) {
        fscanf(fp, "%s", vertex);
        ST_INSERT (graph->hash_table, vertex);
    }
    ST_SHOW(graph->hash_table);
    
    //READING EDGES
    edge_t edge;
    int index_1, index_2;
    while (edge_READ(fp, &edge)) {
        
        //SEARCHING FOR INDICES OF VERTICES IN THE SYMBOL TABLE
        index_1 = ST_SEARCH(graph->hash_table, edge.vertex_START);
        index_2 = ST_SEARCH(graph->hash_table, edge.vertex_END);
        
        //CHECKING IF THE VERTICES EXIST
        if (index_1 >= 0 && index_2 >= 0)
            edge_INSERT (graph, index_1, index_2, edge.weight);
    }
    return graph;
}
void graph_SHOW (graph_t graph) {
    
    //SHOW GRAPH BY ADJACENCY MATRIX
    fprintf(stdout, "- THIS IS THE GRAPH BY ITS ADJACENCY MATRIX:\n\n");
    for (int i = 0; i < graph->N_vertices; i++) {
        fprintf(stdout, "\t%s: ", ST_SEARCH_byIndex(graph->hash_table, i));
        for (int j = 0; j < graph->N_vertices; j++)
            fprintf(stdout, "%d ", graph->Adjacency_Matrix[i][j]);
        fprintf(stdout, "\n");
    }
    
    fprintf(stdout, "\n\n");
    
    //SHOW GRAPH BY ADJACENCY LIST
    fprintf(stdout, "- THIS IS THE GRAPH BY ITS ADJACENCY LIST:\n\n");
    for (int i = 0; i < graph->N_vertices; i++) {
        fprintf(stdout, "\tAdjacency list of vertex %s\n\t\t", ST_SEARCH_byIndex(graph->hash_table, i));
        list_SHOW_R(graph, graph->Adjacency_List[i]);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
    return;
}

//Operation
void graph_REMOVE (graph_t graph, edge_t edge) {
    
    //VARIABLES USED
    int index_START = ST_SEARCH(graph->hash_table, edge.vertex_START);
    int index_END = ST_SEARCH(graph->hash_table, edge.vertex_END);
    
    //REMOVE FROM ADJACENCY LIST
    link x;
    //BYPASS IN "HEAD"
    if (graph->Adjacency_List[index_START]->vertex == index_END) {
        graph->Adjacency_List[index_START] = graph->Adjacency_List[index_START]->next;
        graph->N_edges--;
    }
    //NORMAL BYPASS
    else {
        for (x = graph->Adjacency_List[index_START]; x != graph->z; x = x->next)
            if (x->next->vertex == index_END) {
                x->next = x->next->next;
                graph->N_edges--;
            }
    }
    
    //REMOVE FROM ADJACENCY MATRIX
    if (graph->Adjacency_Matrix[index_START][index_END] != 0)
        graph->Adjacency_Matrix[index_START][index_END] = 0;
}

//Visiting graph functions
void graph_DFS (graph_t graph) {
    
    //VARIABLES USED
    int time = 0;   //Time counter
    int *pre = NULL;   //Array storing the time in which the particular vertex has been discovered
    int *post = NULL;  //Array storing the time in which the particular vertex has been completed (we have gone through every edges with the recursion)
    int *st = NULL;    //Array storing the index reference to the parent of a particular vertex
    
    
    //DYNAMIC ALLOCATION OS SPACE IN MEMORY FOR ARRAYS
    pre = (int*) malloc(graph->N_vertices * sizeof(int));
    post = (int*) malloc(graph->N_vertices * sizeof(int));
    st = (int*) malloc(graph->N_vertices * sizeof(int));
    if (pre == NULL || post == NULL || st == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING ARRAYS
    for (int i = 0; i < graph->N_vertices; i++) {
        pre[i] = -1;
        post[i] = -1;
        st[i] = -1;
    }
    
    //STARTING DEPTH-FIRST VISIT OF THE GRAPH FROM INDEX
    fprintf(stdout, "[EDGES TYPE]: \n");
    for (int V_index = 0; V_index < graph->N_vertices; V_index++)
        if (pre[V_index] == -1)
            graph_DFS_R (graph, edge_CREATE(graph, V_index, V_index, 0), &time, pre, post, st);
    fprintf(stdout, "\n");
    
    //SHOWING RESULTS
    fprintf(stdout, "[VERTEX]: DISCOVERY/END_PROCESSING\n");
    for (int i = 0; i < graph->N_vertices; i++)
        fprintf(stdout, "\t[%s]: %d/%D\n", ST_SEARCH_byIndex(graph->hash_table, i), pre[i], post[i]);
    fprintf(stdout, "\n");
    
    fprintf(stdout, "[RESULTING DFS TREE]:\n");
    for (int i = 0; i < graph->N_vertices; i++) {
        fprintf(stdout, "\t[%s]'s parent: [%s]\n", ST_SEARCH_byIndex(graph->hash_table, i), ST_SEARCH_byIndex(graph->hash_table, st[i]));
    }
    fprintf(stdout, "\n\n");

    //FREE
    free(pre);
    free(post);
    free(st);
    
    return;
}

//Find the minimum set of edges in the original graph that can be removed to form a DAG
edge_t* edgeSet_GraphToDAG_SEARCH (graph_t graph, int *minimum_set_cardinality) {
    
    //CHECK IF THE GRAPH IS ALREADY A DAG
    int N_backEdges = graphDAG_CHECK(graph);
    if (N_backEdges == 0) {
        fprintf(stdout, "- CHECK IF THE GRAPH IS A DAG: The graph is already a DAG.\n\n");
        return NULL;
    }
 
    /* --- FIRST ATTEMPT: RECURSION ALGOTIRHM THAT CONSIDER EVERY SET OF EDGES, REMOVE IT AND CHECK IF THE GRAPH IS A DAG --- */
    
    //DYNAMIC ALLOCATION FOR SOLUTION ARRAY
    long double N_set_MAX = binomialCoefficient(graph->N_edges, N_backEdges);
    edge_t **edges_ToBeRemoved = (edge_t**) malloc(N_set_MAX * sizeof(edge_t));
    if (edges_ToBeRemoved == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N_set_MAX; i++) {
        edges_ToBeRemoved[i] = (edge_t*) malloc(N_backEdges * sizeof(edge_t));
        if (edges_ToBeRemoved[i] == NULL) {
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
        
        //INITIALIZING
        for (int j = 0; j < N_backEdges; j++) {
            strcpy(edges_ToBeRemoved[i][j].vertex_START, "VOID");
            strcpy(edges_ToBeRemoved[i][j].vertex_END, "VOID");
            edges_ToBeRemoved[i][j].weight = -1;
        }
    }
    edge_t *edges_TMP = (edge_t*) malloc(graph->N_edges * sizeof(edge_t));
    if (edges_TMP == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //FILLING SET OF ITEMS (EDGES)
    edge_t *edges = edge_FILL(graph);
    
    //COPYING GRAPH IN ORDER TO REMOVE AND INSERT EDGE WITHOUT EDITING THE ORIGINAL GRAPH
    graph_t graph_TMP = graph_INIT(graph->N_vertices);
    graph_COPY(&graph_TMP, graph);
    
    //RECURSIVELY COMPUTING EDGES TO BE REMOVED
    int edge_count = 0; //Count how many edges has been considered in the solution
    int count = 0;  //Count how many solution we have found
    
    //COMPUTING SOLUTION
    edgeSet_GraphToDAG_SEARCH_R(graph_TMP, edges_TMP, edges, edges_ToBeRemoved, 0, &count, &edge_count, &N_backEdges, 0);
    
    //FREE ARRAYS
    free(edges);
    free(edges_TMP);
    int N_set = count;
    
    
    //SHOWING RESULTS
    fprintf(stdout, "- MINIMUM SET OF EDGES THAT HAS TO BE REMOVED IN ORDER TO FORM A DAG:\n");
    int flag = 0;
    int index_SET = 0;
    for (int i = 0; i < N_set; i++) {
        int j = 0;
        if (strcmp(edges_ToBeRemoved[i][j].vertex_START, "VOID") != 0)
            fprintf(stdout, "SET %d:", ++index_SET);
        while (j < N_backEdges && strcmp(edges_ToBeRemoved[i][j].vertex_START, "VOID") != 0) {
            fprintf(stdout, "\t[%s %s] ", edges_ToBeRemoved[i][j].vertex_START, edges_ToBeRemoved[i][j].vertex_END);
            ++j;
            flag = 1;
        }
        
        if (flag == 1)
            fprintf(stdout, "\n");
        flag = 0;
    }
    
    
    
    //SEARCHING FOR THE MAXIMUM WEIGHTED SET OF EDGES AND RETURNING
    //VARIABLE USED
    int weight_MAX = 0;
    int weight_TMP = 0;
    int index = -1;
    
    //LOOKING AT EVERY SOLUTION SET
    for (int i = 0; i < N_set; i++) {
        
        //RESETTING WEIGHT
        weight_TMP = 0;
        
        //COMPUTING TOTAL WEIGHT
        for (int j = 0; j < N_backEdges; j++) {
            weight_TMP = weight_TMP + edges_ToBeRemoved[i][j].weight;
        }
        
        //UPLOADING MAXIMUM VALUE AND INDEX OF SOLUTION SET
        if (weight_TMP > weight_MAX) {
            index = i;
            weight_MAX = weight_TMP;
        }
    }
    
    //RETURNING SOLUTION
    (*minimum_set_cardinality) = N_backEdges;
    return edges_ToBeRemoved[index];
}

//Find longest path from each source to every other vertex in the graph
void graph_longestPath_SEARCH (graph_t graph) {
    
    //CREATING ARRAY FOR STORING TOPOLOGICAL SORT
    int *array_TS = (int*) malloc(graph->N_vertices * sizeof(int));
    if (array_TS == NULL) {
        fprintf(stdout, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    int *prev = (int*) malloc(graph->N_vertices * sizeof(int));
    if (prev == NULL) {
        fprintf(stdout, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING VALUES
    for (int i = 0; i < graph->N_vertices; i++) {
        prev[i] = -1;
        array_TS[i] = -1;
    }
    
    //SORTING GRAPH IN REVERSE TOPOLOGICAL ORDER
    int time = 0;
    for (int V_index = 0; V_index < graph->N_vertices; V_index++)
        if (prev[V_index] == -1)
            graphDAG_DFS_TS (graph, V_index, array_TS, prev, &time);
    
    //REVERSE ARRAY
    int *array_TS_R = (int*) malloc(graph->N_vertices * sizeof(int));
    if (array_TS_R == NULL) {
        fprintf(stdout, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = graph->N_vertices-1, j = 0; i >= 0; i--, j++)
        array_TS_R[j] = array_TS[i];
    
    //SHOWING RESULTS
    fprintf(stdout, "- THOSE ARE THE LONGEST PATH FROM EVERY SOURCE VERTEX TO EVERY OTHER VERTEX IN THE GRAPH:\n\n");
    
    //VISITING EVERY VERTEX...
    for (int i = 0; i < graph->N_vertices; i++) {
        
        //...IN TOPOLOGICAL ORDER
        int V_index = array_TS_R[i];
        
        //CHECKING IF IT IS A SOURCE NODE BY INSPECTION IN THE ADJACENCY MATRIX
        if (source_CHECK(graph, V_index)) {
            fprintf(stdout, "\t[STARTING NODE: %s]:\n", ST_SEARCH_byIndex(graph->hash_table, V_index));
            
            //FINDING LONGEST PATH FROM THAT SOURCE TO EVERY OTHER VERTEX
            longestPath_SEARCH_R (graph, V_index, array_TS_R);
            
        }
        fprintf(stdout, "\n");
         
    }
    
    //FREE
    free(array_TS_R);
    free(array_TS);
    free(prev);
    
    return;
}
