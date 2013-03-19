#include "cilk-bfs.h"





int cilk_main(int argc, char* argv[]) {
/*
	int SCALE = 10;
	int edgefactor = 16;
	int NBFS = 64;
	int *tail, *head;
	

	if (argc == 1) {
    	startvtx = atoi (argv[1]);
  	} else {
    	printf("usage:   bfs500\n");
	    printf("example: cat sample.txt | ./bfstest 1\n");
	    exit(1);
  	}


	rand ("seed", 103);

	kronecker_generator (SCALE, edgefactor, &tail, &head);

	tic;
	G = kernel_1 (ij);
	kernel_1_time = toc;

	N = size (G, 1);
	coldeg = full (spstats (G));
	search_key = randperm (N);
	search_key(coldeg(search_key) == 0) = [];
	if length (search_key) > NBFS,
	  search_key = search_key(1:NBFS);
	else
	  NBFS = length (search_key);
	end
	search_key = search_key - 1;  

	kernel_2_time = Inf * ones (NBFS, 1);
	kernel_2_nedge = zeros (NBFS, 1);

	indeg = histc (ij(:), 1:N); % For computing the number of edges

	for k = 1:NBFS,
	  tic;
	  parent = kernel_2 (G, search_key(k));
	  kernel_2_time(k) = toc;
	  err = validate (parent, ij, search_key (k));
	  if err <= 0,
	    error (sprintf ("BFS %d from search key %d failed to validate: %d",
	                    k, search_key(k), err));
	  end
	  kernel_2_nedge(k) = sum (indeg(parent >= 0))/2; % Volume/2
	end

	output (SCALE, edgefactor, NBFS, kernel_1_time, kernel_2_time, kernel_2_nedge);
*/


}

