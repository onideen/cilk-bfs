#include "cilk-bfs.h"
#include "run_details.cpp"
using namespace std;

double getTimeInMillis() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000000.0+ t.tv_usec;  
}

int read_edge_list (int **tailp, int **headp) {
  int max_edges = 100000000;
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
  int i, e, v;
  G = (graph *) calloc(1, sizeof(graph));
  G->ne = nedges;

  int maxv;
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

bool hasNeighours(int startvtx, graph *G ) {
  return G->firstnbr[startvtx] != G->firstnbr[startvtx+1];
}

void walkNeighbourNodes(int v, VertexBag *writeBag, int *level, int *parent, int thislevel, graph *G){
  int w, e;
  // /VertexBag writeBag = (VertexBag )
  for (e = G->firstnbr[v]; e < G->firstnbr[v+1]; e++) {
    w = G->nbr[e];          // w is the current neighbor of v
    writeBag->addEdge();
    if (level[w] == -1) {   // w has not already been reached
      parent[w] = v;
      level[w] = thislevel+1;
      writeBag->push(w);    // put w on queue to explore   
    
    }
  }
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


VertexBag *splitAndMergeBag(VertexBag *inbag, int start, int end, int *level, int *parent, int thislevel, graph *G ){
  if ((end - start) > 128) {
    VertexBag *leftbag = new VertexBag();
    VertexBag *rightbag = new VertexBag();
    int mid = (end + start) / 2;
    leftbag = cilk_spawn splitAndMergeBag(inbag, start, mid, level, parent, thislevel, G);
    rightbag = splitAndMergeBag(inbag, mid+1, end, level, parent, thislevel, G);
    cilk_sync;

    leftbag->mergeBags(rightbag);
    free(rightbag);
    return leftbag;
  }
  else {
    VertexBag* bag = new VertexBag();
    for (int i = 0; i <= end-start; i++) {
      int v = inbag->getElement(start+i);
      walkNeighbourNodes(v, bag, level, parent, thislevel, G);
    }
     return bag;

  }

}

void bfs (int s, graph *G, int **levelp, int *nlevelsp, int **levelsizep, int **parentp) {
  int *level, *levelsize, *parent;
  int thislevel;
  int back, front;
  int nedges = 0;
  VertexBag *readBag = new VertexBag();

  int *queue;

  levelsize = *levelsizep = (int *) calloc(G->nv, sizeof(int));
  level = *levelp = (int *) calloc(G->nv, sizeof(int));
  parent = *parentp = (int *) calloc(G->nv, sizeof(int));
  
  for (int v = 0; v < G->nv; v++) level[v] = -1;
  for (int v = 0; v < G->nv; v++) parent[v] = -1;

  // assign the starting vertex level 0 and put it on the queue to explore
  thislevel = 0;
  level[s] = 0;
  levelsize[0] = 1;
  readBag->push(s);

  // loop over levels, then over vertices at this level, then over neighbors
  while (! readBag->isEmpty()) {
    levelsize[thislevel+1] = 0;
    VertexBag *bag;

    bag = cilk_spawn splitAndMergeBag(readBag, 0, readBag->size() - 1, level, parent, thislevel, G);
    cilk_sync;
    bag->printBag();

    free(readBag);

    nedges += bag->getNedges();
    readBag = bag;


    levelsize[thislevel+1] = readBag->size();
    thislevel = thislevel+1;
  }
  *nlevelsp = thislevel;
}





int cilk_main (int argc, char* argv[]) {
  graph *G;

  int NBFS = 64;
  int *level, *levelsize, *parent;
  int *tail, *head;
  int nedges;
  int nlevels;
  int startvtx;
  int i, v, reached;
  RunDetails* runDetails;

  if (argc == 2) {
    NBFS = atoi (argv[1]);
  } 
  //else {
//    printf("usage:   bfstest <numberOfSearches> < <edgelistfile>\n");
//    printf("example: cat sample.txt | ./bfstest 1\n");
//    exit(1);
  //}

  runDetails = new RunDetails(NBFS, 20, 16);
  nedges = read_edge_list (&tail, &head);
  G = graph_from_edge_list (tail, head, nedges);
  free(tail);
  free(head);
  print_CSR_graph (G);


  while (NBFS > 0) {
    double t1, t2;
    startvtx = rand() % G->nv;
    if (!hasNeighours(startvtx, G)){
      continue;
    }
    NBFS--;
    #ifdef NORMAL
    printf("Starting vertex for BFS is %d.\n\n",startvtx);
    #endif

    t1 = getTimeInMillis();
    bfs (startvtx, G, &level, &nlevels, &levelsize, &parent);
    t2 = getTimeInMillis();

    printf("T1: %20.17e\n", t1);
    printf("T1: %20.17e\n", t2);


    #ifdef NORMAL
    reached = 0;
    for (i = 0; i < nlevels; i++) reached += levelsize[i];
    printf("Breadth-first search from vertex %d reached %d levels and %d vertices.\n",
      startvtx, nlevels, reached);
    for (i = 0; i < nlevels; i++) printf("level %d vertices: %d\n", i, levelsize[i]);
    if (G->nv < 30) {
      printf("\n  vertex parent  level\n");
      for (v = 0; v < G->nv; v++) printf("%6d%7d%7d\n", v, parent[v], level[v]);
    }
    printf("\n");
    #endif

    free(level);
    free(levelsize);
    free(parent);
  }

}

