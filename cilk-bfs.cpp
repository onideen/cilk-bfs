// BFSTEST : Test breadth-first search in a graph.
// 
// example: cat sample.txt | ./bfstest 1
//

#include <iostream>
#include "bag.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int NTHREADS = 1;


typedef struct graphstruct { // A graph in compressed-adjacency-list (CSR) form
  int nv;            // number of vertices
  int ne;            // number of edges
  int *nbr;          // array of neighbors of all vertices
  int *firstnbr;     // index in nbr[] of first neighbor of each vtx
} graph;


int read_edge_list (int **tailp, int **headp) {
  int max_edges = 1000000;
  int nedges, nr, t, h;
  *tailp = (int *) calloc(max_edges, sizeof(int));
  *headp = (int *) calloc(max_edges, sizeof(int));
  nedges = 0;
  nr = scanf("%i %i",&t,&h);
  while (nr == 2) {
    if (nedges >= max_edges) {
      printf("Limit of %d edges exceeded.\n",max_edges);
      exit(1);
    }
    (*tailp)[nedges] = t;
    (*headp)[nedges++] = h;
    nr = scanf("%i %i",&t,&h);
  }
  return nedges;
}


graph * graph_from_edge_list (int *tail, int* head, int nedges) {
  graph *G;
  int i, e, v, maxv;
  G = (graph *) calloc(1, sizeof(graph));
  G->ne = nedges;
  maxv = 0;

  // count vertices
  for (e = 0; e < G->ne; e++) {
    if (tail[e] > maxv) maxv = tail[e];
    if (head[e] > maxv) maxv = head[e];
  }
  G->nv = maxv+1;
  G->nbr = (int *) calloc(G->ne, sizeof(int));
  G->firstnbr = (int *) calloc(G->nv+1, sizeof(int));

  // count neighbors of vertex v in firstnbr[v+1],
  for (e = 0; e < G->ne; e++) G->firstnbr[tail[e]+1]++;

  // cumulative sum of neighbors gives firstnbr[] values
  for (v = 0; v < G->nv; v++) G->firstnbr[v+1] += G->firstnbr[v];

  // pass through edges, slotting each one into the CSR structure
  for (e = 0; e < G->ne; e++) {
    i = G->firstnbr[tail[e]]++;
    G->nbr[i] = head[e];
  }
  // the loop above shifted firstnbr[] left; shift it back right
  for (v = G->nv; v > 0; v--) G->firstnbr[v] = G->firstnbr[v-1];
  G->firstnbr[0] = 0;
  return G;
}


void print_CSR_graph (graph *G) {
  int vlimit = 20;
  int elimit = 50;
  int e,v;
  printf("\nGraph has %d vertices and %d edges.\n",G->nv,G->ne);
  printf("firstnbr =");
  if (G->nv < vlimit) vlimit = G->nv;
  for (v = 0; v <= vlimit; v++) printf(" %d",G->firstnbr[v]);
  if (G->nv > vlimit) printf(" ...");
  printf("\n");
  printf("nbr =");
  if (G->ne < elimit) elimit = G->ne;
  for (e = 0; e < elimit; e++) printf(" %d",G->nbr[e]);
  if (G->ne > elimit) printf(" ...");
  printf("\n\n");
}


void bfs (int s, graph *G, int **levelp, int *nlevelsp, 
         int **levelsizep, int **parentp) {
  int *level, *levelsize, *parent;
  int thislevel;
  int back, front;
  int i, v, w, e;
  VertexBag *readBag = new VertexBag();
  VertexBag *writeBag = new VertexBag();

  int *queue;

  levelsize = *levelsizep = (int *) calloc(G->nv, sizeof(int));
  level = *levelp = (int *) calloc(G->nv, sizeof(int));
  parent = *parentp = (int *) calloc(G->nv, sizeof(int));
  
  //queue = (int *) calloc(G->nv, sizeof(int));

  for (v = 0; v < G->nv; v++) level[v] = -1;
  for (v = 0; v < G->nv; v++) parent[v] = -1;

  // assign the starting vertex level 0 and put it on the queue to explore
  thislevel = 0;
  level[s] = 0;
  levelsize[0] = 1;
  readBag->put(s);


  // loop over levels, then over vertices at this level, then over neighbors
  while (! readBag->isEmpty()) {
    levelsize[thislevel+1] = 0;
    
    readBag->printBag();    
    printf("-1-\n");

    VertexBag* readBags = readBag->split(NTHREADS);
    printf("-2-\n");

    VertexBag* writeBags = new VertexBag[NTHREADS];

    printf("-3-\n");

    for (int i = 0; i < NTHREADS; i++){
      readBags[i].printBag();

      while (! readBags[i].isEmpty()) {
        v = readBags[i].get();       // v is the current vertex to explore from

        for (e = G->firstnbr[v]; e < G->firstnbr[v+1]; e++) {
          w = G->nbr[e];          // w is the current neighbor of v

          if (level[w] == -1) {   // w has not already been reached
            parent[w] = v;
            level[w] = thislevel+1;
            writeBags[i].put(w);    // put w on queue to explore
          }
        }
      }
    }
    
    printf("Merging bags with %i and %i element(s) \n", writeBags[0].size(), writeBags[1].size());
    writeBags[0].mergeBags(writeBags[1]);
    printf("Bag now has %i element(s) \n", writeBags[0].size());
    writeBags[0].printBag();
    readBag = &writeBags[0];

    levelsize[thislevel+1] = readBag->size();

    thislevel = thislevel+1;

  }
  *nlevelsp = thislevel;
}


int cilk_main (int argc, char* argv[]) {
  graph *G;
  int *level, *levelsize, *parent;
  int *tail, *head;
  int nedges;
  int nlevels;
  int startvtx;
  int i, v, reached;

  if (argc == 2) {
    startvtx = atoi (argv[1]);
  } else {
    printf("usage:   bfstest <startvtx> < <edgelistfile>\n");
    printf("example: cat sample.txt | ./bfstest 1\n");
    exit(1);
  }
  nedges = read_edge_list (&tail, &head);
  G = graph_from_edge_list (tail, head, nedges);
  free(tail);
  free(head);
  print_CSR_graph (G);

  printf("Starting vertex for BFS is %d.\n\n",startvtx);
  bfs (startvtx, G, &level, &nlevels, &levelsize, &parent);

  reached = 0;
  for (i = 0; i < nlevels; i++) reached += levelsize[i];
  printf("Breadth-first search from vertex %d reached %d levels and %d vertices.\n",
    startvtx, nlevels, reached);
  for (i = 0; i < nlevels; i++) printf("level %d vertices: %d\n", i, levelsize[i]);
  if (G->nv < 20) {
    printf("\n  vertex parent  level\n");
    for (v = 0; v < G->nv; v++) printf("%6d%7d%7d\n", v, parent[v], level[v]);
  }
  printf("\n");
}
