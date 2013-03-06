#include <iostream>
#include "math.h"

using namespace std;

class VertexBag {

	int bagSize;
	int lengthOfArray;
	int* bagArray;

	/* Where the last vertex is located */
	int counter;



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

	bool exists(int vertex) {
		for (int i = 0; i < size(); i++) {
			if (vertex == bagArray[i]) {
				return true;
			}
		}
		return false;
	}

	void merge(VertexBag bag){
		int sortedArray = new int[this.bagSize + bag.bagSize];
		
		for (int i = 0; i < this.bagSize; ++i){
			if(this.get()>bag.get()){
				sortedArray[2*i] = this
			}
		}
		

	}

	bool isEmpty(){
		return size() == 0;
	}


	void put(int vertex);

	int get(); 

	int getElement(int i);

	void setElement(int i, int value);

	void join(VertexBag otherBag) {
		while (otherBag.size() != 0) {
			put(otherBag.get());
		}
	}

	VertexBag* split(int numberOfBags){

		VertexBag *bags;
		bags = new VertexBag[numberOfBags];
		int numberInEachBag = (int)ceil(size()/numberOfBags);

		for (int i = 0; i < numberOfBags; i++) {
			for (int j = 0; j < numberInEachBag && !isEmpty(); j++){
				bags[i].put(get());
			}
		}
		return bags;

	}
};

VertexBag::VertexBag() {
	lengthOfArray = 500;
	bagArray = new int[lengthOfArray];
	counter = -1;
	bagSize = 0;
}
void VertexBag::put(int vertex) {
	
	if (size() == 0){
		setElement(0, vertex);
	}
	for (int i = 0; i < size(); i++) {
		if (vertex < getElement(i)){
			int prev = getElement(i);
			setElement(i, vertex);
			bagSize++;
			for(int j = i; j < size(); j++) {
				int tmp = getElement(j);
				setElement(j, prev);
				prev = tmp;
			}
			break;
		}
	}
}
int VertexBag::get() {
	bagSize--;
	return bagArray[bagSize];
}

int VertexBag::getElement(int i) {
	return bagArray[i];
}

void VertexBag::setElement(int i, int value) {
	bagArray[i] = value;
}
