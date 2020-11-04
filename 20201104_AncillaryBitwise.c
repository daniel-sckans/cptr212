#include <stdlib.h>
#include <stdio.h>

void print_bits(char c) {
    for(int i = 1; i < 256; i *= 2) {
        printf("%d", c & i ? 1 : 0); 
    }
}

int main(int argc, char** argv) {

    char C = 10; 
    print_bits(C ^ 4); 


    return EXIT_SUCCESS; 
}