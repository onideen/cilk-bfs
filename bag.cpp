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

	void mergeBags(VertexBag bag){
		int* sortedArray = new int[bagSize + bag.bagSize];
		int* left = new int[bagSize];
		int* right = new int[bag.bagSize];

		//for sorterte lister
		for (int i = 0; i < bagSize; ++i){
			left[i] = get();
		}

		for (int i = 0; i < bag.bagSize; ++i){
			right[i] = bag.get();
		}

		sortedArray = mergeLists(left,right,bagSize,bag.bagSize);
		
		//for usorterte lister, har sannsynligvis bug
		/*
		while(!this.isEmpty()){
			sortedArray = get();
		}
 		while(!bag.isEmpty()){
			sortedArray = bag.get();
		}

		sortedArray = merge_sort(sortedArray,bagSize + bag.bagSize);
		
		*/
		
		for (int i = 0; i < bagSize + bag.bagSize; ++i){
			put(sortedArray[i]);
		}
	}

	int* mergeLists(int *a, int *b, int sizeA, int sizeB){
		int* result = new int[sizeA+sizeB];
		int l =0 ,r =0;
		
		for (int i = 0; i < sizeA + sizeB; ++i){
			if(a[l] == b[r] && l < sizeA && r < sizeB){
				result[i] = a[l++];
				r++;
				sizeB--;
			}else if(a[l] < b[r] && l < sizeA && r < sizeB){
				result[i] = a[l++];
			}else if(b[r] < a[l]  && l < sizeA && r < sizeB){
				result[i] = b[r++];
			}else if(l >= sizeA){
				result[i] = b[r++];
			}else if(r >= sizeB){
				result[i] = a[l++];
			}
		}

		return result;
	}

	//Sannsynligvis bug med size hvis det er duplicates
	int* merge_sort(int *list, int size){
		int middle = ceil(size/2);
		int* left = new int[(int)ceil(size/2)];
		int* right = new int[(int)floor(size/2)];

		if(size <=1){
			return list;
		}

		for (int i = 0; i < size; ++i){
			if(i> ceil(size/2)){
				left[i] = list[i];
			}else{
				right[i-(int)ceil(size/2)] = list[i];
			}
		}
		left = merge_sort(left,ceil(size/2));
		right = merge_sort(right,floor(size/2));
		return mergeLists(left,right,ceil(size/2),floor(size/2));

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
	int index;
	int min = 0, max = bagSize-1;

	bagSize++;
	if (size() == 0){
		setElement(0, vertex);
		return;
	}else if(vertex > bagArray[bagSize-2]){
		setElement(bagSize-1,vertex);
		return;
	}
	
	while(max > min){
		index = ceil((max + min)/2);
	
		if(vertex > bagArray[index]){
			min = index + 1;
		}else if(vertex < bagArray[index]){
			max = index -1;
		}else{
			//Number is in bag, sould not happen!
		}
	}

	if (min>max)	{
		index = min;
	}if(min=max){
		index = min+1;
	}

	int prev = getElement(index);
	setElement(index, vertex);
	for(int j = index+1; j < size(); j++) {
		int tmp = getElement(j);
		setElement(j, prev);
		prev = tmp;
	}
	return;
}
int VertexBag::get() {
	if(bagSize != 0){
		return bagArray[--bagSize];
	}else{
		return -1;
	}
}

int VertexBag::getElement(int i) {
	return bagArray[i];
}

void VertexBag::setElement(int i, int value) {
	bagArray[i] = value;
}
