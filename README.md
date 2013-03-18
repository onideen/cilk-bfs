cilk-bfs
========

Parallel Breadth-First Search written in Cilk++




Bags
======

our bags supports  
´put()´ adds vertex to bag, if it not exists  
´get()´ gets the next vertex 
´join()´ joins two bags and eliminats duplictes  
´split()´ splits the bag into multiple smaller bags  



Problems
=========

  * Bags with more than 3500 elements




Future
======
  * Bags as heaps
  * Figure out how to alloc 3500 elements
  * Graph 500 implementation
  * Cilkify make graph