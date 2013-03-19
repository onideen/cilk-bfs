#include "cilk-bfs.cpp"

using namespace std;



int cilk_main (int argc, char* argv[]) {
  graph *G;

  int NBFS = 64;
  int *level, *levelsize, *parent;
  int *tail, *head;
  int nedges;
  int nlevels;
  int startvtx;
  int i, v, reached;

  if (argc != 1) {
    printf("usage:   bfstest < <edgelistfile>\n");
    printf("example: cat sample.txt | ./bfstest 1\n");
    exit(1);
  }
  nedges = read_edge_list (&tail, &head);
  G = graph_from_edge_list (tail, head, nedges);
  free(tail);
  free(head);
  print_CSR_graph (G);


  while (NBFS > 0) {

    startvtx = rand() % G->nv;
    if (!hasNeighours(startvtx, G)){
      continue;
    }
    NBFS--;
    printf("Starting vertex for BFS is %d.\n\n",startvtx);
    bfs (startvtx, G, &level, &nlevels, &levelsize, &parent);

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
  }
}

