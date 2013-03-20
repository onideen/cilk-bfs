all: cilk-bfs bfstest bfs500 rmat

bfstest: bfstest.c
	gcc bfstest.c -o bfstest

cilk-bfs: cilk-bfs.cpp cilk-bfs.h bag.cpp run_details.cpp
	cilk++ -DNORMAL cilk-bfs.cpp -o bfs -lrt

bfs500: cilk-bfs.cpp cilk-bfs.h bag.cpp run_details.cpp
	cilk++ -DGRAPH500 cilk-bfs.cpp -o bfs500 -lrt

rmat: rmat.c
	cc -o rmat rmat.c

clean: 
	rm bfs bfstest bfs500 rmat
