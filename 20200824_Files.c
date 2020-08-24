#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

int main(int argc, char* argv[argc+1]) {

    // Get the terminal window size.  
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // Check that one argument was supplied.  
    // The first argument is always the command running the program, e.g. "./a.out".  
    if(argc != 2) {
        fprintf(stderr, "You must enter exactly one argument naming the file.\n"); 
        return EXIT_FAILURE; 
    }

    // Get the file supplied as an argument.  
    // If opening the file fails – for instance, if the file name was wrong – the variable named "txt_file" will be NULL.  
    // We can check against that (check to see if it is NULL) with "!txt_file".  
    FILE* txt_file = fopen(argv[1], "r"); 
    if(!txt_file) {
        fprintf(stderr, "The argument you entered does not correspond to a correct file.\n"); 
        return EXIT_FAILURE; 
    }

    // Print one line of text for each row in the terminal except the last (to prevent scrolling once the program exits).  
    // We use a buffer (think of it as an array for us to temporarily hold data); it has to have room at the end for the terminating '\0' at the end of a string (AKA array of chars).  
    // "fgets()" stores the current line (up to a given length, here "ws.ws_col", or the number of columns in our terminal) of the file into the buffer, and then moves to the next line.  
    for(int y = 0; y < ws.ws_row - 1; y++) {
        char buffer[ws.ws_col + 1]; 
        fgets(buffer, ws.ws_col, txt_file); 
        printf("%s", buffer); 
    }

    // No failure.  
    fclose(txt_file); 
    printf("\n"); 
    return EXIT_SUCCESS; 
}