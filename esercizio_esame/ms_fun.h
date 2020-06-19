#ifndef MSFUN_H
#define MSFUN_H

struct ms_struct{
    int* arr;
    int l;
    int r;
}

void merge_sort(int* arr, int l, int r);
void merge(int* arr, int l, int m, int r);
void print_array(int* arr, int size);
bool is_sorted(int* arr, int size);

#endif