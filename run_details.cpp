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

	double *startvtx;
	double *runtime;
	double *nedges;
	double *depth;

	double *TEPS;

	int counter;


public:
	RunDetails(int NBFS, int SCALE, int edgefactor);

	void addConstructionTime(double constructionTime);
	void addRun(double startvtx, double runtime, double nedges, double depth);

	double minRuntime();
	double maxRuntime();

	double minNedges();
	double maxNedges();

	double minTEPS();
	double maxTEPS();

	void printStatistics();
};

RunDetails::RunDetails(int NBFS, int SCALE, int edgefactor) {
	this->NBFS = NBFS;
	this->SCALE = SCALE;
	this->edgefactor = edgefactor;

	startvtx = new double[NBFS];
	runtime = new double[NBFS];
	nedges = new double[NBFS];
	depth = new double[NBFS];

	TEPS = new double[NBFS];
	counter = 0;
}

void RunDetails::addRun(double startvtx, double runtime, double nedges, double depth) {
	this->startvtx[counter] = startvtx;
	this->runtime[counter] = runtime;
	this->nedges[counter] = nedges;
	this->depth[counter] = depth;
	this->TEPS[counter++] = nedges/runtime;
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
double RunDetails::minNedges() {
	double min = nedges[0];
	for (int i = 1; i < counter; i++){
		if (nedges[i] < min ) min = nedges[i];
	}
	return min;
}

double RunDetails::maxNedges() {
	double max = nedges[0];
	for (int i = 1; i < counter; i++){
		if (nedges[i] > max ) max = nedges[i];
	}
	return max;
}

double RunDetails::minTEPS() {
	double min = TEPS[0];
	for (int i = 1; i < counter; i++){
		if (TEPS[i] < min ) min = TEPS[i];
	}
	return min;
}

double RunDetails::maxTEPS() {
	double max = TEPS[0];
	for (int i = 1; i < counter; i++){
		if (TEPS[i] > max ) max = TEPS[i];
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

  	printf("min_nedges: %20.17e\n", minNedges());
  	printf("max_nedges: %20.17e\n", maxNedges());


  	printf("min_TEPS: %20.17e\n", minTEPS());
  	printf("max_TEPS: %20.17e\n", maxTEPS());


}
