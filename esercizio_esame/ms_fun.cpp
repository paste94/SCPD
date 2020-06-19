#include <iostream>
#include "ms_fun.h"
using namespace std;

/*  
    args: int* arr, int l, int r
*/
void* merge_sort(void* args) 
{   

    if (l < r) 
    {
        // Find central index
        int m = (l+r)/ 2; 
  
        // Sort first and second halves
        merge_sort(arr, l, m); 
        merge_sort(arr, m + 1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int* arr, int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
  
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


void print_array(int* arr, int size) 
{ 
    int i; 
    for (i = 0; i < size; i++) 
        cout << arr[i] << "\n";  
} 

bool is_sorted(int* arr, int size)
{
    for(int i = 1; i < size; i++)
    {
        if(arr[i-1] > arr[i])
        {
            return false;
        }
    }
    return true;
}