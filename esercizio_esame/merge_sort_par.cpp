#include <iostream>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

#define MAX_SIZE 100000000
#define FILE "./rand_int_20.txt"


void merge(int arr[], int size, int temp[]);
void mergesort_serial(int arr[], int size, int temp[]);
void merge_sort_parallel(int arr[], int size, int temp[], int threads);
void print_array(int arr[], int l, int r);
bool is_sorted(int arr[], int size);

using namespace std;

int main(int argc, char * argv[])
{   
    if (argc != 2)
    {
        printf("Usage: %s num_threads\n", argv[0]);
        return 1;
    }
    int n_threads = atoi(argv[1]);
    //cout << "\n--- New parallel computation ---\n";
    cout << n_threads << ",";

    static int arr[MAX_SIZE]; // Array containing elements to sort
    int size=0; // Index of the last element of arr
    static int temp[MAX_SIZE];

    // ----- Load array from file -----//
    std::fstream myfile(FILE, std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        arr[size] = a;
        size++;
    }

    //print_array(arr, 0, size);

    // ----- Sort array -----//
    omp_set_num_threads(n_threads);
    
    auto t1 = chrono::high_resolution_clock::now();
    omp_set_nested(1);
    merge_sort_parallel(arr, size, temp, n_threads);  
    auto t2 = chrono::high_resolution_clock::now();

    chrono::duration<double,milli> elapsed = t2 - t1;

    // ----- Statistics -----//
    /*
    if(is_sorted(arr, size)){
        cout << "Array sorted. \n";
    }else{
        cout << "Array NOT sorted. \n";
    }
    */
    //cout << "FILE: " << FILE << "\nTIME: " << elapsed.count() << " milliseconds.\n\n";
    cout << elapsed.count() << "\n";
    return 0;
}


// OpenMP merge sort with given number of threads
void merge_sort_parallel(int arr[], int size, int temp[], int threads) {
    if (threads == 1) {
        mergesort_serial(arr, size, temp);
    }
    else if (threads > 1) {
        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                #pragma omp task
                {
                    merge_sort_parallel(arr, size / 2, temp, threads / 2);
                }
                #pragma omp task
              	{
                    merge_sort_parallel(arr + size / 2, size - size / 2,
                                           temp + size / 2, threads - threads / 2);
                }
		        #pragma omp taskwait
                {
                    merge(arr, size, temp);
                }
            }
        }
    }
    else {
        printf("Error: %d threads\n", threads);
        return;
    }
}

// only called if num_threads = 1
void mergesort_serial(int arr[], int size, int temp[]) {
    if (size>1) 
    {
        mergesort_serial(arr, size / 2, temp);
        mergesort_serial(arr + size / 2, size - size / 2, temp);
        merge(arr, size, temp);
    }

}

void merge(int arr[], int size, int temp[]) {
    int i1 = 0;
    int i2 = size / 2;
    int tempi = 0;
    while (i1 < size / 2 && i2 < size) {
        if (arr[i1] < arr[i2]) {
            temp[tempi] = arr[i1];
            i1++;
        } else {
            temp[tempi] = arr[i2];
            i2++;
        }
        tempi++;
    }
    while (i1 < size / 2) {
        temp[tempi] = arr[i1];
        i1++;
        tempi++;
    }
    while (i2 < size) {
        temp[tempi] = arr[i2];
        i2++;
        tempi++;
    }
    // Copy sorted temp array into main array, a
    memcpy(arr, temp, size * sizeof(int));
}


void print_array(int arr[], int l, int r) 
{ 
    int i; 
    cout << "[ ";
    for (i = l; i < r; i++) 
        cout << arr[i] << ", "; 
    cout << " ]\n";
} 

bool is_sorted(int arr[], int size)
{
    bool b = true;
    for(int i = 1; i < size; i++)
    {
        if(arr[i-1] > arr[i]){
            cout << arr[i-1] << ">" << arr[i]<< " at index "<<i<<"\n\n";
            //print_array(arr, size);
            cout << "\n";
            b=false;
        }
    }
    return b;
}