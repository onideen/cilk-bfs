#include "cilk-bfs.h"
#include "run_details.cpp"
#define getRandom() (drand48())
using namespace std;

double getTimeInMicroSec() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000000.0+ t.tv_usec;  
}



void randPerm(int n, uint32_t perm[])
{
  int i, j, t;

  for(i=0; i<n; i++)
    perm[i] = i;

  for(i=0; i<n; i++) {
    j = rand()%(n-i)+i;
    t = perm[j];
    perm[j] = perm[i];
    perm[i] = t;
  }
}

int read_edge_list (int **tailp, int **headp) {
  int max_edges = 300000000;
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

int generateEdges(int SCALE, int edgefactor, uint32_t *head, uint32_t *tail){ 
  
  uint32_t N =  (((uint32_t)1) << SCALE); // Set the number of vertices
  
  uint32_t M = (edgefactor * N); // Set the number of edges

  double const A = 0.57;
  double const B = 0.19;
  double const C = 0.19;
  


  double const ab = (A+B); 
  double const c_norm = C / (1 - ab);
  double const a_norm = A / ab;

  uint32_t ib;
  uint32_t randNum;

  // Set the seeds for the drand48 and the rand function
  srand48((long int)time(NULL));
  srand((long int)time(NULL));

  int ii_bit, jj_bit;

  uint32_t j;
  printf("Generating edgelist with scale %d and edgefactor %d \n",SCALE,edgefactor );
  for(ib = 1; ib <= SCALE; ib++) {
      
    for(j = 0; j < M; j++) {
      ii_bit = (getRandom() > ab);
      jj_bit = (getRandom() > (c_norm * ii_bit + a_norm * !(ii_bit)));
      tail[j] += ((uint32_t)1 << (ib - 1)) * ii_bit;
      head[j] += ((uint32_t)1 << (ib - 1)) * jj_bit;
    }
  }
  
  uint32_t * p = (uint32_t *) malloc(N*sizeof(uint32_t)); 
  if(p == NULL) {
    printf("Malloc failed for permutation array p\n");
    exit(-1);
  }
  
  // Permute the vertices
  randPerm(N,p);

  for(j = 0; j < M; j++) {
    uint32_t index = tail[j];
    tail[j] = p[index];
    index = head[j];
    head[j] = p[index];
  }

  free(p);  
    

  printf("Finished generating %d edges\n", M);
  return M;
}



graph * graph_from_edge_list (uint32_t *tail, uint32_t *head, uint32_t nedges) {
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

void bfs (int s, graph *G, int **levelp, int *nlevelsp, int **levelsizep, int **parentp, double* nedgest) {
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

    free(readBag);

    nedges += bag->getNedges();
    readBag = bag;


    levelsize[thislevel+1] = readBag->size();
    thislevel = thislevel+1;
  }
  *nlevelsp = thislevel;
  *nedgest = nedges;
}

void display_help(){
  printf("Usage: bfs [Options]\n" );
  printf("BFS Search in Cilk\n");

  printf("  -r     Reads from stdin\n");
  printf("  -g     Generates random edges (default mode)\n");
  printf("  -s     Scales, 2^Scales number of vertices (default 20)\n");
  printf("  -e     Edgefactor, average number of endges for each vertex\n");
  printf("  -n     The number of nodes to search from (default 64)\n");
  printf("  -h     Prints this helpfile\n");
}


int cilk_main (int argc, char* argv[]) {
  graph *G;

  int NBFS = 64;
  int *level, *levelsize, *parent;
  uint32_t *tail, *head;
  uint32_t nedges;
  int nlevels;
  int startvtx;
  int scale, edgefactor;
  int i, v, reached;
  int opt = 0;
  RunDetails* runDetails;
  uint32_t M;
  bool reading = false;
  scale = 20;
  edgefactor = 16;

  opt = getopt(argc, argv, optString);
  while( opt != -1){
    switch( opt){
      case 'g':
          reading = false;
        break;
      case 'h':
        display_help();
        return 0;
      case 'r':
        reading = true;
        break;
      case 's':
         scale = atoi(optarg);
        break;
      case 'e':
          edgefactor = atoi(optarg);
        break;
      case 'n':
        //nuber of tests to run
        NBFS = atoi(optarg);
        break;
      case '?':
        printf("Invalid command line\n");
        display_help();
        exit(1);
        break;
    }
    
    opt = getopt(argc, argv, optString);

  }
  

  printf("Options, Scale %d, Edgefactor: %d, Reading %d, NBFS %d \n", scale, edgefactor, reading, NBFS);
  //else {
//    printf("usage:   bfstest <numberOfSearches> < <edgelistfile>\n");
//    printf("example: cat sample.txt | ./bfstest 1\n");
//    exit(1);
  //}
  //default values

  M = (((uint32_t)1) << scale) * edgefactor;

  runDetails = new RunDetails(NBFS, scale, edgefactor);
 // nedges = read_edge_list (&tail, &head);
  tail = (uint32_t *) malloc(M*sizeof(uint32_t));
  head = (uint32_t *) malloc(M*sizeof(uint32_t));

  nedges = generateEdges(scale,edgefactor,head,tail);
  G = graph_from_edge_list (tail, head, nedges);
  free(tail);
  free(head);
  print_CSR_graph (G);


  while (NBFS > 0) {
    double t1, t2;
    double nedgest;

    startvtx = rand() % G->nv;
    if (!hasNeighours(startvtx, G)){
      continue;
    }
    NBFS--;
    printf("Starting on vertex for BFS is %d.  Runs left: %d\n\n",startvtx, NBFS);

    t1 = getTimeInMicroSec();
    bfs (startvtx, G, &level, &nlevels, &levelsize, &parent, &nedgest);
    t2 = getTimeInMicroSec();

    runDetails->addRun(startvtx, t2-t1, nedgest, nlevels);

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

  #ifdef GRAPH500
    runDetails->printStatistics();

  #endif

}

