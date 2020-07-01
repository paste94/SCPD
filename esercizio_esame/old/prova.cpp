#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <iostream>

#define MAX 10

int main() {
   std::cout<<omp_get_num_procs()<<"\n";
}