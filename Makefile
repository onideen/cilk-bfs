all: cilk-bfs bfstest bfs500

bfstest: bfstest.c
	gcc bfstest.c -o bfstest

cilk-bfs: cilk-bfs.cpp 
	cilk++ cilk-bfs.cpp -o bfs

bfs500: bfs500.cpp
	cilk++ -DGRAPH500 bfs500.cpp -o bfs500


clean: 
	rm bfs bfstest bfs500