#include <iostream>
#include "bag.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

typedef struct graphstruct { // A graph in compressed-adjacency-list (CSR) form
  int nv;            // number of vertices
  int ne;            // number of edges
  int *nbr;          // array of neighbors of all vertices
  int *firstnbr;     // index in nbr[] of first neighbor of each vtx
} graph;

static const char *optString = "hgrs:e:n:";

void bfs (int s, graph *G, int **levelp, int *nlevelsp, int **levelsizep, int **parentp);
int read_edge_list (int **tailp, int **headp);
graph * graph_from_edge_list (int *tail, int* head, int nedges);
void print_CSR_graph (graph *G);
bool hasNeighours(int startvtx, graph *G );



