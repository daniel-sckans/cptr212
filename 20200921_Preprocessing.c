#include <stdlib.h>
#include <stdio.h>

#define statement "Program has run.\n"

#define print_statement printf(statement) 

#define NEW_FOR_BEGIN for(int i = 0; i < 100; i++) {
#define NEW_FOR_END }

#define ADD(a, b) a + b

int main(int argc, char* argv[argc + 1]) {

    printf(statement); 

    NEW_FOR_BEGIN
        print_statement; 
    NEW_FOR_END

    printf("%d", ADD(5, 9)); 

    return EXIT_SUCCESS; 
}