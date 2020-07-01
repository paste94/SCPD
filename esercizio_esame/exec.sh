#!/bin/bash

g++ -Wall -fopenmp -std=c++11 merge_sort_par.cpp -o ms_par.o
for i in {1..56} do echo "Welcome $i times" done