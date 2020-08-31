#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[argc + 1]) {

    // ANSI escape codes are used to control how text is printed to the terminal.  
    // Here, we'll demonstrate how to use them in C.  
    // For a more complete list, visit https://en.wikipedia.org/wiki/ANSI_escape_code.  

    // Basic principlies: 

    // We print escape codes using the same tools we've been talking about (like printf).  
    // When we print an escape code, the letter won't show up (as long as everything works correctly).  
    // However, it will change the text that follows.  

    // All ANSI codes in C will start with "\033[".  
    // After that will follow some numbers, each separated by a semicolon ";".  
    // Lastly comes a single character that tells the terminal how to interpret the numbers.  
    
    // Print normal text (for reference).  
    printf("It was the best of times, it was the blurst of times.\n"); 
    
    // Now, let's use an ANSI code to change the color.  
    // Always start with "\033["
    // "38;2;" tells the terminal it's the foreground color we want to change.  
    // The next three numbers set the red, green, and blue values of the color (they must be between 0-255).  
    // The "m" at the end tells the terminal that we want to change the color.  
    printf("\033[38;2;0;255;255m"); 
    printf("It was the best of times, it was the blurst of times.\n"); 

    // Change the background color now.  
    printf("\033[48;2;0;0;255m"); 
    printf("It was the best of times, it was the blurst of times."); 

    // Reset the color.  
    printf("\033[0m"); 
    printf("\n"); 
    printf("It was the best of times, it was the blurst of times.\n"); 

    // Bold text.  
    printf("\033[1m"); 
    printf("It was the best of times, it was the blurst of times.\n"); 
    printf("\033[0m"); 

    // Move the cursor forward.  
    printf("\033[8C"); 
    printf("It was the best of times, it was the blurst of times.\n"); 

    // You can even move the cursor up and down.  
    printf("\n"); 
    printf("It was the best of times, "); 
    printf("\033[1A"); 
    printf("it was the blurst "); 
    printf("\033[1B"); 
    printf("of times.\n"); 

    return EXIT_SUCCESS; 
}