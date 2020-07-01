#include <iostream>
#include <fstream>
#include <chrono>

#define MAX_SIZE 100000100
#define FILE "./rand_int_100M.txt"

void merge_sort(int* arr, int l, int r);
void merge(int* arr, int l, int m, int r);
void print_array(int* arr, int size);
bool is_sorted(int* arr, int size);

using namespace std;

int main(int argc, char * argv[])
{   
    std::time_t today = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    cout << "\n--- New sequential computation ---\n" << std::ctime(&today);
    static int arr[MAX_SIZE]; // Array containing elements to sort
    int size=0; // Index of the last element of arr

    // ----- Load array from file -----//
    std::fstream myfile(FILE, std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        arr[size] = a;
        size++;
    }

    // ----- Sort array -----//
    auto t1 = chrono::high_resolution_clock::now();
    merge_sort(arr, 0, size);
    auto t2 = chrono::high_resolution_clock::now();

    chrono::duration<double,milli> elapsed = t2 - t1;

    // ----- Statistics -----//

    if(is_sorted(arr, size)){
        cout << "Array sorted. \n";
    }else{
        cout << "Array NOT sorted. \n";
    }

    cout << "FILE: " << FILE << "\nTIME: " << elapsed.count() << " milliseconds.\n\n";

    return 0;
}


/*  
    l: left
    r: right
*/
void merge_sort(int* arr, int l, int r) 
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
    static int L[MAX_SIZE/2], R[MAX_SIZE/2]; 
  
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