#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[argc + 1]) {
    int my_int = 10000; 

    printf("my_int: %d | it's at memory address: %p | the next memory address is: %p\n", my_int, &my_int, &my_int + 1); 

    my_int = 0xff; 
    printf("my_int again: %d\n", my_int); 

    // bytes 8 bits
    // 64 bits

    // Memory address format
    // Series of two-character bytes
    // Each character is between 0 and F: 0 1 2 3 4 5 6 7 8 9 A B C D E F 
    // Each byte can then hold 256 different values, between 0 and 255
    // 0xFF == (16)(16)

    // 64 = 2^6
    // 128 = 2^7
    // 256 = 2^8
    
    // 8 bits in a byte, so 2^8 means each byte can go up to 256 different values.  

    return EXIT_SUCCESS; 
}