#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 128

void quick_sort(); 

int main(int argc, char** argv) {

    int array[ARRAY_SIZE] = {0}; 
    srand(time(NULL)); 
    for(int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 200 - 100; 
        printf("%4d", array[i]); 
    }
    printf("\n\n"); 

    quick_sort(array, array + ARRAY_SIZE - 1); 

    for(int i = 0; i < ARRAY_SIZE; i++) {
        printf("%4d", array[i]); 
    }
    printf("\n"); 

    return EXIT_SUCCESS; 
}

void swap_two(int* a, int* b) {
    int temp = *a; 
    *a = *b; 
    *b = temp; 
}

void quick_sort(int* beginning, int* end) {
    if(end <= beginning) {
        return; 
    }

    int* cursor = beginning; 
    int* pivot = end; 

    for(int* i = beginning; i < end; i++) {
        if(*i <= *pivot) {
            swap_two(i, cursor); 
            cursor++; 
        }
    }
    swap_two(cursor, pivot); 

    quick_sort(beginning, cursor - 1); 
    quick_sort(cursor + 1, end); 
}