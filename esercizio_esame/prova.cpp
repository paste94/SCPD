#include <iostream>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(){
    int a[15];
    for(int i = 0; i < 15; i++){
        a[i]=i;
    }

    int b[15];
    b = a;

    for(int i = 0; i < 15; i++){
        cout << b[i] << "; ";
    }
    cout << "\n";
}