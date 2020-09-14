#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[argc + 1]) {

    // Declaring variables the way we normally do.  
    // These variables are kept in the stack section of our memory.  
    // Because they're in the stack, memory is automatically allocated and deallocated for them, and we just have to worry about declaring them.  
    int i = 500; 
    i++; 
    printf("i: %d\n", i); 
    printf("address of i: %p\n", &i); 
    printf("next address after i: %p\n", &i + 1); 
    printf("size of i: %ld\n", sizeof(i)); 

    // Now we'll create a variable that's a pointer.  
    // Because it refers to a preexisting variable (namely, "i" that we created before), it behaves simply like it "points to" the address of the first variable.  
    // The "*" operator, when used after a type name (e.g. "int*" or "char*"), creates a pointer to that type of variable.  
    // The "*" operator, when used before the name of a variable ("e.g. "*i_p", "*variable") dereferences the value; it points to the value at that address.  
    int* i_p = &i; 
    printf("i: %d\n", *i_p); 
    printf("address of i: %p\n", i_p); 
    printf("next address after i: %p\n", i_p + 1); 
    printf("size of i: %ld\n", sizeof(*i_p)); 

    return EXIT_SUCCESS; 
}