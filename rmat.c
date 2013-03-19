#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#define getRandom() (drand48())

void randPerm(int n, int perm[])
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

int main(int argc, char ** argv) {
	
	if(argc != 3) {
		printf("Usage : %s <SCALE> <edgefactor>\nThe number of vertices is N = 2^SCALE, and  the number of edges is M = edgefactor*N\n", argv[0]);
		exit(-1);
	}

	int SCALE = atoi(argv[1]);
	double edgefactor = atof(argv[2]);
	
	// Set the number of vertices 
	uint32_t N =  (((uint32_t)1) << SCALE);
	
	// Set the number of edges
	uint32_t M = (edgefactor * N);

	double const A = 0.57;
	double const B = 0.19;
	double const C = 0.19;

	uint32_t *ij[2];
	
	ij[0] = (uint32_t *) malloc(M*sizeof(uint32_t));
	ij[1] = (uint32_t *) malloc(M*sizeof(uint32_t));

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
	for(ib = 1; ib <= SCALE; ib++) {
			
		for(j = 0; j < M; j++) {
			ii_bit = (getRandom() > ab);
			jj_bit = (getRandom() > (c_norm * ii_bit + a_norm * !(ii_bit)));
			ij[0][j] += ((uint32_t)1 << (ib - 1)) * ii_bit;
			ij[1][j] += ((uint32_t)1 << (ib - 1)) * jj_bit;
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
		uint32_t index = ij[0][j];
		ij[0][j] = p[index];
		index = ij[1][j];
		ij[1][j] = p[index];
	}

	free(p);	
	p = (uint32_t *) malloc(M*sizeof(uint32_t)); 

	// Permute the edges
	randPerm(M,p);	

	// Print out the final matrix in the edge format or MATLAB format
	#ifdef MATLAB
	printf("function [MAT] = rmat()\n\n");
	printf("MAT = zeros(%u,%u);\n",N,N);
	#else
	printf("0 %u %u\n",N,M);
	#endif
	
	for(j = 0; j < M; j++) {
		uint32_t index = p[j];
		
		#ifdef MATLAB
			printf("\tMAT(%u,%u) = 1;\n",ij[0][index]+1,ij[1][index]+1);
			printf("\tMAT(%u,%u) = 1;\n",ij[1][index]+1,ij[0][index]+1);
		#else
			printf("%u %u\n",ij[0][index],ij[1][index]);
		#endif
	}
	
	#ifdef MATLAB
		printf("\nend\n");
	#endif
}
