#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Enumerated values for the customer location.  
// They increment by one, starting at the last number that was set (or zero, if nothing was specified).  
enum {
    CUST_LOC__WINFIELD, // 0
    CUST_LOC__WICHITA, // 1
    CUST_LOC__DERBY, // 2
    CUST_LOC__KANSAS_CITY, // 3
    CUST_LOC__ARKANSAS_CITY = 10, // 10
    CUST_LOC__MIAMI_BEACH // 11
}; 

// Very simple struct for customer information.  
// Structs can also include other structs, allowing for more complicated data types.  
struct customer {
    char name[64]; 
    char id[16]; 
    int location; 
}; 

// A function to set customer information to default values.  
// This version will fail, but the reason why is very relevant to us.  
// When a variable is passed to a function, the function gets a copy of the variable (it doesn't get access to the original).  
// So setting the information here will not change the original.  
void set_default_customer_0(struct customer c) {
    strcpy(c.name, "Jane Doe"); 
    strcpy(c.id, "123412341234123"); 
    c.location = CUST_LOC__WICHITA; 
}

// Another function to set customer information to default values.  
// This one works, but only because we'll return the copy of the variable.  
struct customer set_default_customer_1(struct customer c) {
    strcpy(c.name, "Jane Doe"); 
    strcpy(c.id, "123412341234123"); 
    c.location = CUST_LOC__WICHITA; 
    return c; 
}


int main(int argc, char* argv[argc + 1]) {

    struct customer jane; 

    // First attempt: set the variable only within the function (this will fail).  
    set_default_customer_0(jane); 
    printf("Customer: name: %s, id: %s, loc: %d\n", jane.name, jane.id, jane.location); 

    // Second attempt: set the variable by getting a return value from the function (this will succeed, but it isn't always the best way of doing things).  
    jane = set_default_customer_1(jane); 
    printf("Customer: name: %s, id: %s, loc: %d\n", jane.name, jane.id, jane.location); 

    return EXIT_SUCCESS; 
}