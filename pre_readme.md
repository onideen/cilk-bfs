# Breadth First Search in Cilk++ 



## Project Introduction

For our project we did a bfs implementation in Cilk++ by using the graph500 specification. The breadth-first-search is an 
algorithm that explores the vertecies and edges of a graph, beginning from a particular starting vertex. To do BFS in a sequential 
algorithm is a remarkably simple operation, but to do it in parallel makes it challenging. The way we solved the problem was to split 
the nodes we wanted to traverse into a bag in each thread walk the nodes and add unvisited nodes to the bag. Then we needed to 
merge all our bags before doing the next level.


A nicer version of this file can be found on http://github.com/vegaen/cilk-bfs

## Table of Contents

@@TOC@@



## BFS

The breadth-first-search is an algorithm that explores the vertecies and edges of a graph, beginning from a particular starting vertex. After bfs is done it returns a tree of the graph,
which tells you how far away any node is from the root node.  
To do this sequencial this is a rally simple problem since the algoritm is really easy:

	procedure BFS(G,v):
		create a queue Q
		enqueue v onto Q
		mark v
		while Q is not empty:
			t ← Q.dequeue()
			if t is what we are looking for:
				return t
			for all edges e in G.adjacentEdges(t) do
				u ← G.adjacentVertex(t,e)
				if u is not marked:
				mark u
				enqueue u onto Q


When you shall make this problem a parallel problem the main issue is how you shall manage the queue. When you in a sequential program can add a new node to the back of the queue 
and keep going, you can't do the same in a parallel program since you have to keep track of which nodes that have been visited.  
A high order approach to the problem is to split the queue between the processors and walk the neighbours of that vertex and put new obtained nodes into a shared next level queue
and wait for every processor to be finished before you start on the next level. 




?? ADD BFS EXPLAINATION IMAGES ??  


## Graph500 ##
Graph500 is a rating of supercomputer system, with focus on Data intensive loads. Instead of counting double precision floating-point as the benchmark, Graph500 are using a breadth-first
Search as the benchmark. In the benchmark it is two computation kernels. The first kernel computes the time it takes to generate the graph and compress it into 
sparse structures CSR or CSC (Compressed Sparse Row/Column). The second kernel does a parallel BFS of 64 random vertecies per run.  
Six possible scales, or sizes, are defined:
 * __Toy__: 2^26 vertices -> 17 GB of RAM
 * __Mini__: 2^29 -> 137 GB of RAM
 * __Small__: 2^32 -> 1,1 TB of RAM
 * __Medium__: 2^36 -> 17,6 TB of RAM)
 * __Large__: 2^39 -> 140 TB
 * __Huge__: 2^42; 1.1 PB of RAM    


## Implementation ##

### Bag Structure ###
We used a long time before we was able to decide on the structure we wanted to use for our bag. After a while we figured out our bag needed certain properties:
  * No duplicates
  * Fast insert
  * Fast merge
  * Fast split

As far as we coudn't find any single data structur that fitted all of this properties, and most of them fit one or two properties, but are horrible at the others. For example
a heap is really easy to insert into, but are not able to reject duplicates. While it is quite easy to secure a binary three against duplicates, but again a insert is really expencive.
The solution we ended up with was quite clever, each processor gets it own bag which is a heap, but when they they merge the bag structure becomes a sorted list. 



#### No duplicates ####
Since we mark a visited node, a single processor cannot try to add duplicates to the bag. The only way to get duplicates is it two (or more) different processors access read the same node
at the same time and think it isn't visited earlier. By using this knowledge we do not need to handle duplicates for one single bag -> we can insert as a heap. 


#### Fast insert ####
Since insert is an operation we do often, it is crucial that it is fast. Wheater or not the bag structure is a heap, it will always insert an element at the end, but if it is a heap it will
min-heapify afterwards. So the insert operation will ether be O(1) or O(log n), which is reasonably fast.


#### Get min-value fast ####
Since we need the min-value of a bag in the merge it is crucial that getting the min-value is reasonably fast. When the bag is a heap, it is O(1) to peek at the min-value, but it is O(log n)
to pop it. When the bag is a sorted list it is O(1) to peek and pop. 



#### Fast merge #####
When we merge we do kind of a merge sort, we compare the two smallest elements in each of the bags and inserts the smallest element in a new bag (which is actually one of the old bags, to
save initiation time of a new bag). If the smallest element of each bag is equal only one value is added, but we go further in both of the bags. When both of the bags are heaps a merge operation is O(n+m+log n + log m), and when both is sorted list a merge operation is O(n+m).



#### Fast split #####
Since we dont care how to split the vertecies between the processors, we do a really simple split operation. Processor 1 gets the first 1/nproc part of the vertecies and so on. Then the split operation is O(1)




Overall our bag structure is fast enough.


No Reducers.










## Build and Run

`make`

## Results




## Problems
  * We had a lot of problems when we tried tp take the time of the graph search and since all the search was sub-second, it was hard to get an accurate timer. 
	Timing 



## Conclusion



## Future Work




## Sources
  * [http://graph500.org](http://graph500.org "Graph500 official")
  * [http://en.wikipedia.org/wiki/Graph500](http://en.wikipedia.org/wiki/Graph500)