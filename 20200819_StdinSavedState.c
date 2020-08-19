#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[argc+1]) {

    // See /20200812_2dArrays.c
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // This time, we'll save the characters in a 2D array, so the user can change them gradually over time. 
    // We create the array, and then have to initialize each element of it (which we do in this case with nest for-loops).  
    // We'll alter the array (and print from it) after getting user input.  
    char char_grid[ws.ws_row][ws.ws_col]; 
    for(int y = 0; y < ws.ws_row; y++) {
        for(int x = 0; x < ws.ws_col; x++) {
            char_grid[y][x] = 'a'; 
        }
    }

    // If user input is required, you should tell your users what they can enter.  
    printf("Enter 'a' if you want me to draw a cat.\n"); 
    printf("Enter 'b' to . . .\n"); 
    printf("Enter 'q' to quit.\n"); 

    // This is the "main loop" of our program; instead of just exiting, it will continue getting user input until 'q' is entered.  
    // Because of the parentheses, "(c = getchar())" is evaluated first (to the value of the character that is entered).  
    // See our textbook for a table of operator precedence in C.  
    // The function getchar() is what's known as an IO (in-out) function, and the program will "wait" until the user enters input.  
    char c; 
    while((c = getchar()) != 'q') {
        
        // getchar() will also get '\n' (newline) characters; we will ignore it.  
        if(c != '\n') { 

            // We can now alter the data in the array based on user input.  
            // This is just one example, but the array can be changed in any way you'd like.  
            // This doesn't need to be a separate for-loop either, but can be implemented below.  
            // In this case, if the user presses 'a', the fourth column will go up a letter.  
            if(c == 'a') {
                for(int y = 0; y < ws.ws_row; y++) {
                    char_grid[y][3]++; 
                }
            }

            // Iterate through each character of the terminal.  
            for(int y = 0; y < ws.ws_row; y++) {
                for(int x = 0; x < ws.ws_col; x++) {

                    // This time, we just have to print the entire array (the changes have already been made).  
                    printf("%c", char_grid[y][x]); 
                }
            }
        }
    }
}