#include <stdlib.h>
#include <stdio.h>

void print_bits(char c) {
    for(int i = 1; i < 256; i *= 2) {
        printf("%d", c & i ? 1 : 0); 
    }
}

int main(int argc, char** argv) {

    print_bits(11); 
    printf("\n"); 
    print_bits(5); 
    printf("\n"); 
    print_bits(11 ^ 5); 
    printf("\n"); 
    
    return EXIT_SUCCESS; 
}