all: cilk-bfs bfstest bfs500 rmat

bfstest: bfstest.c
	gcc bfstest.c -o bfstest

cilk-bfs: cilk-bfs.cpp 
	cilk++ cilk-bfs.cpp -o bfs

bfs500: bfs500.cpp
	cilk++ bfs500.cpp -o bfs500

rmat: rmat.c
	cc -o rmat rmat.c

clean: 
	rm bfs bfstest bfs500 rmat
