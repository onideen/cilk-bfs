# Breadth First Search in Cilk++ 

@@TOC@@


## Project Introduction

For our project we did a bfs implementation in Cilk++ by using the graph500 specification. The breadth-first-search is an 
algorithm that explores the vertecies and edges of a graph, beginning from a particular starting vertex. To do BFS in a sequential 
algorithm is a remarkably simple operation, but to do it in parallel makes it challenging. The way we solved the problem was to split 
the nodes we wanted to traverse into a bag in each thread walk the nodes and add unvisited nodes to the bag. Then we needed to 
merge all our bags before doing the next level.


Graph500 is a standarized messurement of how fast a progam is on a specific computer to run bfs for large graphs. To do this we implemented a bfs-algoritm in Cilk++ 


## BFS

The breadth-first-search is an algorithm that explores the vertecies and edges of a graph, beginning from a particular starting vertex. This problem is really simple to do
in a sequencial program, but is a lot harder in 


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

  * Bags with more than 3500 elements




## Future

  * Bags as heaps
  * Figure out how to alloc 3500 elements
  * Graph 500 implementation
  * Cilkify make graph