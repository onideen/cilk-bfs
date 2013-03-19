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
	float constructionTime;

	int startvtx*;
	float runtime*;
	int nedges*;
	int depth*;

	int counter;


public:
	RunDetails(int NBFS, int SCALE, int edgefactor);

	void addConstructionTime(float constructionTime);
	void addRun(int startvtx, float runtime, int nedges, int depth);

	float minRuntime();
	float maxRuntime();



	void printStatistics();
};

RunDetails::RunDetails(int NBFS, int SCALE, int edgefactor) {
	this->NBFS = NBFS;
	this->SCALE = SCALE;
	this->edgefactor = edgefactor;

	startvtx = new int[NBFS];
	runtime = new float[NBFS];
	nedges = new int[NBFS];
	depth = new int[NBFS];
	counter = 0;
}

void RunDetails::addRun(int startvtx, int runtime, int nedges, int depth) {
	this->startvtx[counter] = startvtx;
	this->runtime[counter] = runtime;
	this->nedges[counter] = nedges;
	this->depth[counter++] = depth;
}


void RunDetails::addConstructionTime(int constructionTime) {
	this->constructionTime = constructionTime;
}

float RunDetails::minRuntime() {
	float min = runtime[0];
	for (int i = 1; i < conter; i++){
		if (runtime[i] < min ) min = runtime[i];
	}
	return min;
}

float RunDetails::maxRuntime() {
	float max = runtime[0];
	for (int i = 1; i < conter; i++){
		if (runtime[i] > max ) max = runtime[i];
	}
	return max;
}

void RunDetails::printStatistics(){
	printf("SCALE: %d\n", SCALE);
	printf("edgefactor: %d\n", edgefactor);
  	printf("NBFS: %d\n", counter);
//  	printf("construction_time: %20.17e\n", constructionTime); 

  	printf("min_time: %20.17e\n", minTime());
  	printf("max_time: %20.17e\n", maxTime());

}
