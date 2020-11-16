#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

// Same as in previous samples (get one character without pressing return/enter).  
char get_char() {
    struct termios original_term, new_term;
    tcgetattr(STDIN_FILENO, &original_term);
    new_term = original_term;
    new_term.c_lflag &= ~ICANON;
    new_term.c_lflag &= ~ECHO;
    tcsetattr(TCSANOW, STDIN_FILENO, &new_term);
    char c = getchar();
    tcsetattr(TCSANOW, STDIN_FILENO, &original_term);
    return c;
}

int main(int argc, char* argv[argc+1]) {

    // Same as the text reader with buffer sample.  
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    FILE* txt_file = fopen(argv[1], "r");
    if(!txt_file) {
        fprintf(stderr, "That was a mistaken file opening session.\n");
        return EXIT_FAILURE;
    }
    int lines_in_file = 0;
    while(!feof(txt_file)) {
        char buf[ws.ws_col+1];
        fgets(buf, ws.ws_col, txt_file);
        lines_in_file++;
    }
    fseek(txt_file, 0, SEEK_SET);
    char* txt[lines_in_file];
    for(int y = 0; y < lines_in_file; y++) {
        txt[y] = (char*)malloc(ws.ws_col * sizeof(char) + 1);
        fgets(txt[y], ws.ws_col + 1, txt_file);
    }

    // File editor functionality.  

    // First, we'll print out the text one time.  
    int line_offset = 0; 
    for(int y = 0; y < ws.ws_row; y++) {
        printf("%s", txt[y + line_offset]);
    }

    // "x" and "y" are variables to hold our position on the screen.  
    // The screen is not zero-ordered, so we'll start out with them at (1, 1).  
    int x = 1;
    int y = 1;
    
    // This will send the cursor to position (1, 1).  
    printf("\033[1;1H"); 

    char c = '\0';
    while((c = get_char()) != 'q') {

        // All of this is just to monitor for input from arrow keys.  
        // Arrow keys start with the ANSI escape sequence "\033[" and then a letter (A through D).  
        // First, we check for the \033 (which is an octal — or base-eight — character).  
        if(c == '\033') {

            // Then, we throw away the '[' (we don't need it, we already know we're dealing with an escape sequence).  
            getchar();

            // Finally we worry about the escaped character.  
            // For iterating through many contingencies, we should use a switch() instead of long if/else-if/else blocks.  
            // The functionality is the same, but a switch is clearer.  
            char control_char = getchar(); 
            switch(control_char) {
                case 'A':
                    if(1 < y) {
                        y--;
                    }
                    break;
                case 'B':
                    if(y < ws.ws_row) {
                        y++;
                    }
                    break;
                case 'C':
                    if(x < ws.ws_col) {
                        x++;
                    }
                    break;
                case 'D':
                    if(1 < x) {
                        x--;
                    }
                    break;

                case '5': 
                case '6': 

                    // Yet another throwaway char.  
                    getchar(); 

                    // Go up or down.  
                    // This uses the ternary operator ("... ? ... : ..." pattern) for brevity; see the textbook or Google if you're not familiar.  
                    line_offset = control_char == '5' ? line_offset - ws.ws_row : line_offset + ws.ws_row; 

                    // Check if the input is okay, then print it.  
                    if (line_offset < 0) {
                        line_offset = 0; 
                    } else if (lines_in_file < line_offset + ws.ws_row) {
                        line_offset = lines_in_file - ws.ws_row; 
                    }
                    printf("\033[1;1H\033[2J"); 
                    for(int y = 0; y < ws.ws_row; y++) {
                        fputs(txt[y + line_offset], stdout); 
                    }
                    break; 
            }
        } else {

            // If we weren't moving around the text, we want to replace it.  
            // We'll change the color of the text, because it is clearer, and because "why not".  
            printf("\033[38;2;0;255;255m%c\033[0m", c);
            txt[y + line_offset - 1][x++ - 1] = c;
        }

        // We'll use the 'H' ANSI escape code to go to a position.  
        // Flush the output to make sure it writes to the screen.  
        printf("\033[%d;%dH", y, x);
        fflush(stdout);
    }

    // Newline, erase text after the cursor.  
    printf("\n\033[J"); 
    fflush(stdout); 

    // To save the edited file, we just have to print out our buffer.  
    FILE* txt_out = fopen("assets/ATaleOfThreeCities.txt", "w");
    if(!txt_out) {
        fprintf(stderr, "Error writing to file file.\n");
        return EXIT_FAILURE;
    }
    for(int y = 0; y < lines_in_file; y++) {
        fprintf(txt_out, "%s", txt[y]);
    }
    fflush(txt_out);
    fclose(txt_out); 
   
    // Huzzah.  
    return EXIT_SUCCESS;
}

