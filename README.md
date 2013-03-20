# Breadth First Search in Cilk++ 



## Project Introduction

For our project we did a bfs implementation in Cilk++ by using the graph500 specification. The breadth-first-search is an 
algorithm that explores the vertecies and edges of a graph, beginning from a particular starting vertex. To do BFS in a sequential 
algorithm is a remarkably simple operation, but to do it in parallel makes it challenging. The way we solved the problem was to split 
the nodes we wanted to traverse into a bag in each thread walk the nodes and add unvisited nodes to the bag. Then we needed to 
merge all our bags before doing the next level.


Graph500 is a standarized messurement of how fast a progam is on a specific computer to run bfs for large graphs. To do this we implemented a bfs-algoritm in Cilk++ 


## Table of Contents

- [Breadth First Search in Cilk++](#breadth-first-search-in-cilk)
    - [Project Introduction](#project-introduction)
    - [Table of Contents](#table-of-contents)
    - [BFS](#bfs)
    - [Graph500](#graph500)
    - [Implementation](#implementation)
        - [Bag Structure](#bag-structure)
    - [Build and Run](#build-and-run)
    - [Results](#results)
    - [Problems](#problems)
    - [Conclusion](#conclusion)
    - [Future Work](#future-work)
    - [Problems](#problems)
    - [Sources](#sources)



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


This problem is really simple to do
in a sequencial program, but to do it in parallel is more complicated. The reason behind that is because bfs are making a tree


##Graph500


## Implementation

No Reducers.

### Bag Structure







## Build and Run

`make`


## Results




## Problems




## Conclusion



## Future Work




## Problems

	Timing 


## Sources
