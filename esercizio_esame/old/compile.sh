#!/bin/bash

#g++ -Wall -std=c++11 merge_sort_seq.cpp -o ms_seq.o
g++ -Wall -fopenmp -std=c++11 merge_sort_par.cpp -o ms_par.o

for i in {1..50}; do ./ms_par.o $i >> ms_par.log; done
