#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[argc+1]) {

    // See /20200812_2dArrays.c
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // If user input is required, you should tell your users what they can enter.  
    printf("Enter 'a' if you want me to draw a cat.\n"); 
    printf("Enter 'b' to . . .\n"); 
    printf("Enter 'q' to quit.\n"); 

    // This is the "main loop" of our program; instead of just exiting, it will continue getting user input until 'q' is entered.  
    // Because of the parentheses, "(c = getchar())" is evaluated first (to the value of the character that is entered).  
    // The function getchar() is what's known as an IO (in-out) function, and the program will "wait" until the user enters input.  
    char c; 
    while((c = getchar()) != 'q') {
        if(c != '\n') { // getchar() will also get '\n' (newline) characters.  

            // Iterate through each character of the terminal.  
            for(int y = 0; y < ws.ws_row; y++) {
                for(int x = 0; x < ws.ws_col; x++) {

                    // Print patterns that respond to user input here.  
                    // For this assignment, you __must__ change your output according to user input.  
                    if(c == 'a') { 
                        printf("%c", '.'); 
                    }
                }
            }
        }
    }
}