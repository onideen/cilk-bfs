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
		

	
	return M;
}

int main(int argc, char ** argv) {
	uint32_t *head, *tail;
	uint32_t j;

	uint32_t n = (((uint32_t)1) << 14);
	uint32_t M = n * 16;
	tail = (uint32_t *) malloc(M*sizeof(uint32_t));
	head = (uint32_t *) malloc(M*sizeof(uint32_t));
	
	generateEdges(14,16,head,tail);



	for(j = 0; j < M; j++) {
		uint32_t index = j;
		
		
		printf("%u %u\n",tail[index],head[index]);
	}
}