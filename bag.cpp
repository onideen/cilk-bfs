#include <iostream>
#include "math.h"
#include <string>
#include <fstream>

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

	void mergeBags(VertexBag bag);
	int* mergeLists(int *a, int *b, int sizeA, int sizeB){
		printf("Starting mergeLists\n");
		int* result = new int[sizeA+sizeB];
		int l =0 ,r =0, size;
		
		size = sizeA + sizeB;
		for (int i = 0; i < size; ++i){
		//	printf("Appending %i or %i \n", a[l],b[r]);
			if(a[l] == b[r] && l < sizeA && r < sizeB){
				result[i] = a[l++];
				r++;
				size--;
			}else if(a[l] < b[r] && l < sizeA && r < sizeB){
				result[i] = a[l++];
			}else if(a[l] > b[r]  && l < sizeA && r < sizeB){
				result[i] = b[r++];
			}else if(l >= sizeA){
				result[i] = b[r++];
			}else if(r >= sizeB){
				result[i] = a[l++];
			}
		}
		bagSize = size;
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

	VertexBag* split(int numberOfBags);

	void printBag();
};

VertexBag::VertexBag() {
	lengthOfArray = 500;
	bagArray = new int[lengthOfArray];
	counter = -1;
	bagSize = 0;
}

void VertexBag::printBag(){
	string str("Bag: ");
	cout << "Bag: ";
	for (int i = 0; i <size() && i < 10; i++) {
		cout << getElement(i) << ", ";
	}
	cout << "\n";
}

void VertexBag::put(int vertex) {
	int index=0;
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
	}if(min==max && vertex < bagArray[min]){
		index = min;
	}else if(min==max){
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

void VertexBag::mergeBags(VertexBag bag){
	printf("Starting mergeBags with %i and %i \n",bagSize,bag.size());
	int* right = new int[bag.size()];
	int test = (int)bagSize+1;
	int* left = new int[10000];
	
	//int* left;
	//left = (int *)calloc(bagSize-1,sizeof(int));
	//sleep(1);
	//for sorterte lister
	printf("Getting left half\n");
	for (int i = 0; i < bagSize; ++i){
		left[i] = getElement(i);
	}

	printf("Getting right half\n");
	for (int i = 0; i < bag.size(); ++i){
		right[i] = bag.getElement(i);
	}

	bagArray = mergeLists(left,right,bagSize,bag.size());

}

VertexBag* VertexBag::split(int numberOfBags){

	VertexBag *bags;
	bags = new VertexBag[numberOfBags];
	int numberInEachBag = (int)ceil((double)size()/numberOfBags);
/*
	for (int i = 0; i < numberOfBags; i++) {

		// lag settere for bagArray og bagSize
		bags[i].bagArray = &bagArray[i*numberInEachBag];
		bags[i].bagSize = numberInEachBag -1;
		if(i < size() % numberInEachBag){
			bags[i].bagSize++;
		}
	}
*/

	for (int i = 0; i < numberOfBags; i++) {
		for (int j = 0; j < numberInEachBag && i*numberInEachBag+j < size(); j++){
			bags[i].put(getElement(i*numberInEachBag + j));
		}
	}
	return bags;


	printf("split done\n");
	return bags;

}
