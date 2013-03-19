#include <iostream>
#include "bag.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <reducer_max.h>

typedef struct graphstruct { // A graph in compressed-adjacency-list (CSR) form
  int nv;            // number of vertices
  int ne;            // number of edges
  int *nbr;          // array of neighbors of all vertices
  int *firstnbr;     // index in nbr[] of first neighbor of each vtx
} graph;


void bfs (int s, graph *G, int **levelp, int *nlevelsp, int **levelsizep, int **parentp);





