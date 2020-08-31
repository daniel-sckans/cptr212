#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>

// See 20200824_Files.c for comments.  
char get_char() {
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
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // This time we'll read the entire file into one big buffer at the beginning, and then we'll print from the buffer.  
    
    // First we loop through the file to see how many lines there are (held in the variable "line_count").  
    // We use !feof() to check that we haven't reached the end of file (EOF).  
    // The variable "_" is a throwaway buffer to store our strings (we don't care about the actual values inside).  
    FILE* in_file = fopen(argv[1], "r"); 
    int line_count = 0; 
    char _[ws.ws_col]; 
    while(!feof(in_file)) {
        fgets(_, ws.ws_col, in_file); 
        line_count++; 
    }
    printf("length: %d\n", line_count); 

    // Next, load each of the lines into our one big buffer.  
    fseek(in_file, 0, SEEK_SET); 
    char buf[line_count][ws.ws_col]; 
    for(int y = 0; y < line_count; y++) {
        fgets(buf[y], ws.ws_col, in_file); 
    }

    char c = '\0'; 
    int line_number = 0; 
    do {
        // Now, going from line to line is as simple as incrementing or decrementing the line number.  
        if(c == 's') {
            line_number++; 
        }
        if(c == 'w' && 0 < line_number) {
            line_number--; 
        }

        // We print from the buffer, not from a temporary buffer set from the file.  
        for(int y = 0; y < ws.ws_row - 1; y++) {
            
            // Here's an example of highlighting an optional search string (passed in as a second argument).  

            // Used in a minute to keep track of the length of the word.  
            int turn_off_highlight_countdown = 0; 
            
            // Print each character one at a time.  
            // Search for the string along the way.  
            for(int x = 0; x < strlen(buf[y + line_number]); x++) {
                if(argv[2]) {
                    char* test_string = argv[2]; 

                    // We've moved forwards a character; this is superfluous if we're not currently highlighting a text, but it does no real harm to our program as decrementing a character is an incredibly quick operation.  
                    turn_off_highlight_countdown--; 

                    // Check if the next letters match the string in question with strncmp ("strncmp" means "string n compare" means "compare the next n characters in two strings").  
                    // We have to use & (the reference operator) in &buf[...] to get the address of each character.  
                    // We will discuss pointers in the next segment.  
                    if(strncmp(test_string, &buf[y + line_number][x], strlen(test_string)) == 0) {

                        // ANSI escape code for bright blue background; we'll also discuss this next week.  
                        printf("\033[48;2;0;0;255m"); 

                        // We've matched the word, so we'll set the countdown to its length.  
                        turn_off_highlight_countdown = strlen(test_string); 
                    } 

                    // We've reached the end of the string, turn off highlighting.  
                    if(turn_off_highlight_countdown == 0) {
                        printf("\033[0m"); 
                    }
                }

                // Finally, print the character.  
                printf("%c", buf[y + line_number][x]); 
            }

            if(buf[y + line_number][strlen(buf[y + line_number]) - 1] != '\n') {
                printf("\n"); 
            }
        }
    } while((c = get_char()) != 'q'); 

    fclose(in_file); 

    return EXIT_SUCCESS; 
}