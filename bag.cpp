#include <iostream>
#include "math.h"
#include <string>
#include <fstream>

using namespace std;

class VertexBag {

	int bagSize;

	int lengthOfArray;
	int* bagArray;

	/* Where the min vertex is located */
	int counter;
	int nedges;
	bool heap;

public:
	/**
	*	Initalizes the Bag
	*/
	VertexBag(); //Constructor


	/**
	*	Returns the number of vertices in the bag
	*/
	int size() {
		return bagSize;
	}

	int getNedges(){
		return nedges;
	}
	void addEdge() {
		nedges++;
	}

	int peekMin();
	int popMin();

	void minHeapify(int pos);

	void mergeBags(VertexBag *bag);

	void put(int vertex) {
		push(vertex);
	}

	void push(int vertex);

	int getElement(int i);

	void printBag();

	bool isEmpty(){
		return size() == 0;
	}
};

VertexBag::VertexBag() {
	lengthOfArray = 1;
	bagArray = new int[lengthOfArray];
	counter = -1;
	bagSize = 0;
	heap = true;
}

void VertexBag::printBag(){
	cout << "Bag: ";
	for (int i = 0; i <size() && i < 10; i++) {
		cout << bagArray[i] << ", ";
	}
	cout << "\n";
}

void VertexBag::push(int vertex) {
	int tmp;
	int i = bagSize;

	if (bagSize == lengthOfArray){
		int *newArray = new int[lengthOfArray*2];
		for (int j = 0; j < lengthOfArray; j++){
			newArray[j] = bagArray[j];
		}
		free(bagArray);
		bagArray = newArray;
		lengthOfArray = lengthOfArray*2;
	}

	bagArray[bagSize++] = vertex;

	while(i > 0 && bagArray[(i-1)/2] > vertex && heap) {
		tmp = bagArray[(i-1)/2];
		bagArray[(i-1)/2] = bagArray[i];
		bagArray[i] = tmp;
		i = (i-1)/2;
	}

}

int VertexBag::peekMin() {
	if (bagSize == 0) return -1;
	if (heap) return bagArray[0];
	else 	return bagArray[counter];
}

int VertexBag::popMin() {
	
	bagSize--;
	if (heap) {
		int min = bagArray[0];

		bagArray[0] = bagArray[bagSize];

		minHeapify(0);
		return min;
	}
	else {
		return bagArray[counter++];
	}

}

void VertexBag::minHeapify(int i){
	int l, r, min;
	l = (i+1)*2-1;
	r = (i+1)*2;

	if (l < bagSize && bagArray[l] < bagArray[i]) {
		min = l;
	} else {
		min = i;
	}
	if (l < bagSize && bagArray[r] < bagArray[i]){
		min = r;
	}

	if (min != i) {
		int tmp = bagArray[min];
		bagArray[min] = bagArray[i];
		bagArray[i] = tmp;

		minHeapify(i);
	}

}

int VertexBag::getElement(int i) {
	return bagArray[i];
}

void VertexBag::mergeBags(VertexBag *bag){
	int *temp;
	temp = new int[size() + bag->size()];
	int i = 0;

	while (!isEmpty() || !bag->isEmpty()){

		if(bag->peekMin() == peekMin() && !isEmpty() && !bag->isEmpty()){
			temp[i] = popMin();
			bag->popMin();
		}else if(!bag->isEmpty() && (bag->peekMin() < peekMin() || isEmpty())) {
			temp[i] =  bag->popMin();
		}else if(peekMin() >= 0){
			temp[i] = popMin();
		}
		i++;
	}

	free(bagArray);
	bagArray = temp;

	bagSize = i;
	counter = 0;
	nedges += bag->getNedges();

	heap = false;	
}
