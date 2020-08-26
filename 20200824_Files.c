#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>

// Custom getchar()-type function to get a single character without the user having to press enter/return.  
// It requires the <termios.h> header.  
// the function gets the information about the terminal and stores it in the variable "old", then creates a copy, "new."
// It then turns off two flags (the "ICANON" AND "ECHO" flags) using bit operators, and sets the terminal to use the new copy.  
// After the text is entered, the terminal is set back to its old version.  
char getcharsingle() {
    struct termios old, new; 
    tcgetattr(0, &old); 
    new = old; 
    new.c_lflag &= ~ICANON; 
    new.c_lflag &= ~ECHO; 
    tcsetattr(0, 0, &new); 
    char c = getchar(); 
    tcsetattr(0, 0, &old); 
    return c; 
}

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

    // When a user enters a character, print out the appropriate range of lines from the text.  
    // Print one line of text for each row in the terminal except the last (to prevent scrolling once the program exits).  
    // We use a buffer (think of it as an array for us to temporarily hold data); it has to have room at the end for the terminating '\0' at the end of a string (AKA array of chars).  
    // "fgets()" stores the current line (up to a given length, here "ws.ws_col", or the number of columns in our terminal) of the file into the buffer, and then moves to the next line.  
    char user_input; 
    long int position = 0; 
    do {
        fseek(txt_file, position, SEEK_SET); 
        char buffer[ws.ws_col + 1]; 
        if(user_input == 's') {
            fgets(buffer, ws.ws_col, txt_file); 
            position = ftell(txt_file); 
        } else if(user_input == 'w') {
            // ???
            // Write some logic that will allow the user the travel to earlier lines in the text.  
        }
        for(int y = 0; y < ws.ws_row - 1; y++) {
            fgets(buffer, ws.ws_col, txt_file); 
            printf("%s", buffer); 
        }
        printf("\n"); 
    } while((user_input = getcharsingle()) != 'q'); 
    
    // No failure.  
    fclose(txt_file); 
    printf("\n"); 
    return EXIT_SUCCESS; 
}