all: cilk-bfs 

cilk-bfs: cilk-bfs.cpp 
	cilk++ cilk-bfs.cpp -o bfs
