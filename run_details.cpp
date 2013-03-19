#include <iostream>
#include "math.h"
#include <string>
#include <fstream>

using namespace std;


/** NOT DONE YET */

class RunDetails {
	int NBFS;
	int SCALE;
	int edgefactor;
	double constructionTime;

	int *startvtx;
	double *runtime;
	int *nedges;
	int *depth;

	int counter;


public:
	RunDetails(int NBFS, int SCALE, int edgefactor);

	void addConstructionTime(double constructionTime);
	void addRun(int startvtx, double runtime, int nedges, int depth);

	double minRuntime();
	double maxRuntime();

	void printStatistics();
};

RunDetails::RunDetails(int NBFS, int SCALE, int edgefactor) {
	this->NBFS = NBFS;
	this->SCALE = SCALE;
	this->edgefactor = edgefactor;

	startvtx = new int[NBFS];
	runtime = new double[NBFS];
	nedges = new int[NBFS];
	depth = new int[NBFS];
	counter = 0;
}

void RunDetails::addRun(int startvtx, double runtime, int nedges, int depth) {
	this->startvtx[counter] = startvtx;
	this->runtime[counter] = runtime;
	this->nedges[counter] = nedges;
	this->depth[counter++] = depth;
}


void RunDetails::addConstructionTime(double constructionTime) {
	this->constructionTime = constructionTime;
}

double RunDetails::minRuntime() {
	double min = runtime[0];
	for (int i = 1; i < counter; i++){
		if (runtime[i] < min ) min = runtime[i];
	}
	return min;
}

double RunDetails::maxRuntime() {
	double max = runtime[0];
	for (int i = 1; i < counter; i++){
		if (runtime[i] > max ) max = runtime[i];
	}
	return max;
}

void RunDetails::printStatistics(){
	printf("SCALE: %d\n", SCALE);
	printf("edgefactor: %d\n", edgefactor);
  	printf("NBFS: %d\n", counter);
//  	printf("construction_time: %20.17e\n", constructionTime); 

  	printf("min_time: %20.17e\n", minRuntime());
  	printf("max_time: %20.17e\n", maxRuntime());

}
