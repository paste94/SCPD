#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "omp.h"
#include <bits/stdc++.h>
#include <chrono>

#define MAX_SIZE 1000000
#define N_THREADS 1
#define FILE "./rand_int_1M.txt"

using namespace std;

void* merge_sort_caller(void* args);
void merge_sort(int arr[], int l, int r);
void print_array(int* arr, int size);
bool is_sorted(int* arr, int size);
void merge(int* arr, int l, int m, int r);
void merge_subarrays(void* arguments, int size);

struct ms_struct{
    int* arr;
    int start; //l
    int end; //r
};

int main(){
    std::time_t today = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    cout << "--- New computation ---\n" << std::ctime(&today);

    int arr[MAX_SIZE]; // Array containing elements to sort
    int size=0; // Index of the last element of arr
    int size_per_proc; // Number of elements for each processor
    pthread_t threads[N_THREADS]; // Threads
    struct ms_struct args[N_THREADS]; // Arguments for threads

    // ----- Load array from file -----//
    std::fstream myfile(FILE, std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        arr[size] = a;
        size++;
    }

    auto t1 = chrono::high_resolution_clock::now();
    //----- Split array -----//
    size_per_proc = size/N_THREADS;
    args[0].arr = arr;
    args[0].start = 0;
    args[0].end = size_per_proc-1;

    for(int i = 1; i < N_THREADS; i++){
        args[i].arr = arr;
        args[i].start = args[i-1].end+1;
        args[i].end = args[i].start + size_per_proc-1;
    }

    args[N_THREADS-1].end = size-1;

    //print_array(arr, size);

    //----- Create thread and sort subarrays -----//
    for(int i = 0; i < N_THREADS; i++){
        //cout << "FROM " << args[i].start << " TO " << args[i].end << "\n";
        pthread_create( &threads[i], NULL, &merge_sort_caller, (void*)&args[i]);
    }

    for(int j=0; j < N_THREADS; j++){
        pthread_join( threads[j], NULL); 
    }

    //print_array(arr, size);

    //----- Merge subarrays -----//
    merge_subarrays(args, size);
    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double,milli> elapsed = t2 - t1;

    //print_array(arr, size);
    
    //----- Check if resulting arrray is sorted -----//
    if(is_sorted(arr, size)){
        cout << "Array sorted. \n";
    }else{
        cout << "Array NOT sorted. \n";
    }

    cout << "FILE: " << FILE << "\nTHREADS: " << N_THREADS << "\nTIME: " << elapsed.count() << " milliseconds.\n\n";
}

/*  
    This function is used for call merge sort on threads
    args: int* arr, int s, int r
*/
void* merge_sort_caller(void* arguments) 
{   
    struct ms_struct *args = (struct ms_struct *)arguments;
    merge_sort(args->arr, args->start, args->end);
    return NULL;
} 

/*  
    Recursive call
*/
void merge_sort(int arr[], int l, int r) 
{ 
    //cout << "l: " << l << " r: " << r << "\n";
    if (l < r) 
    {
        // Find central index
        int m = (l+r)/ 2; 
  
        // Sort first and second halves
        merge_sort(arr, l, m); 

        //cout << "other\n";
        merge_sort(arr, m + 1, r); 

        //cout << "START MERGE\n";
        merge(arr, l, m, r); 
    } 
    //cout << "EXIT " << "l: " << l << " r: " << r << "\n";

} 

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int* arr, int l, int m, int r) 
{ 
    //cout << "l: " << l << " m: " << m << " r: " << r << "\n";
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
    
    //cout << n1 << " - " << n2 << "\n";
    /* create temp arrays */
    int L[n1], R[n2]; 
    //cout<<"PRIMA\n";
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
        //cout<<i<<" -- "<<l+i<<"\n";
        //cout<<arr<<"\n";
        //cout<<i<<" -- "<<l+i<<"\n";
        L[i] = arr[l + i]; 
    }
        

    //cout<<"MEZZO\n";
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
    //cout<<"DOPO\n";
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 

/*
    This function merges all subarrays into one 
*/
void merge_subarrays(void* arguments, int size){
    struct ms_struct *args = (struct ms_struct *)arguments;
    int* arr = args->arr;
    int max_len = args[N_THREADS-1].end - args[N_THREADS-1].start;
    int sub_arr[N_THREADS][max_len]; // Matrix containing tmp arrays
    int index[N_THREADS]; //Indexes of the current used element in sub_array
    int len_arr[N_THREADS]; //Length of each sub_array

    //print_array(arr, size);

    // -- init sub_arr -- //
    for(int i = 0; i < N_THREADS; i++){
        index[i] = 0;
        len_arr[i] = args[i].end - args[i].start + 1; // Temporary arr lenght;

        for(int j = 0; j < len_arr[i]; j++){
            sub_arr[i][j] = arr[args[i].start + j];
        }
    }

    /*
    for(int i = 0; i < N_THREADS; i++){
        cout << "[ ";
        for(int j = 0; j < len_arr[i]; j++){
            cout << sub_arr[i][j] << ", ";
        }
        cout << " ]\n";
    }
    */
    


    // -- Merge sub_arr in arr --//
    for(int i = 0; i < size; i++){
        
        //print_array(index, N_THREADS);
        //print_array(len_arr, N_THREADS);
        int min = INT_MAX;
        int min_index;
        for(int j = 0; j < N_THREADS; j++){
            //cout << index[j] << " " << len_arr[j] << " " << sub_arr[j][index[j]] << " " << min << "\n";
            if(index[j] < len_arr[j] && sub_arr[j][index[j]] < min){
                min = sub_arr[j][index[j]];
                min_index = j;
            }
        }
        //cout << "found " << min << " at index " << min_index << "\n";
        //cout<<"\n";

        arr[i] = min;
        index[min_index]++;
    }

    //print_array(arr, size);

}


void print_array(int* arr, int size) 
{ 
    int i; 
    cout << "[ ";
    for (i = 0; i < size; i++) 
        cout << arr[i] << ", ";  
    cout << "]\n";
} 

bool is_sorted(int* arr, int size)
{
    for(int i = 1; i < size; i++)
    {
        if(arr[i-1] > arr[i]){
            cout << arr[i-1] << ">" << arr[i]<< " at index "<<i<<"\n\n";
            //print_array(arr, size);
            cout << "\n";
            return false;
        }
    }
    return true;
}