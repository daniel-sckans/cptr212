#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>

// See previous samples for description.   
char get_char_raw() {
    struct termios old_terminal, new_terminal; 
    tcgetattr(STDOUT_FILENO, &old_terminal); 
    new_terminal = old_terminal; 
    new_terminal.c_lflag &= ~ICANON; 
    new_terminal.c_lflag &= ~ECHO; 
    tcsetattr(TCSANOW, STDOUT_FILENO, &new_terminal); 
    char c = getchar(); 
    tcsetattr(TCSANOW, STDOUT_FILENO, &old_terminal); 
    return c; 
}

int main(int argc, char* argv[argc + 1]) {

    // See previous samples for description.  
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // Populating the minsweeper data.  

    // Create a buffer to hold the data.  
    int buffer[ws.ws_row][ws.ws_col]; 

    // Populate it.  
    // "srand(time(NULL))" seeds our "rand()" function with the current time.  
    srand(time(NULL)); 
    for(int y = 0; y < ws.ws_row; y++) {
        for(int x = 0; x < ws.ws_col; x++) {
            
            // If the remainder of that random number divided by 0 is 0, save 1 to the buffer.  
            // On average, that will happen one in 25 times.  
            // Otherwise, save 0.  
            buffer[y][x] = rand() % 15 == 0; 

            // Fill up our screen.  
            printf(" "); 
        }
    }

    // Move the cursor to (1, 1).  
    printf("\033[H"); 

    // See 20200904_FileEditor.c for comments on moving the cursor.  
    char c = '\0'; 
    int cursor_x = 0, cursor_y = 0; 
    while((c = get_char_raw()) != 'q') {
        switch(c) {
            case 'w': 
                if(0 < cursor_y) {
                    cursor_y--; 
                }
                break; 
            case 's': 
                if(cursor_y < ws.ws_row - 1) {
                    cursor_y++; 
                }
                break; 
            case 'd': 
                if(cursor_x < ws.ws_col - 1) {
                    cursor_x++; 
                }
                break; 
            case 'a': 
                if(0 < cursor_x) {
                    cursor_x--; 
                }
                break; 
            case 'f': 

                // A bomb was present and the user clicked it, so the game is over.  
                if(buffer[cursor_y][cursor_x] == 1) {
                    printf("\033[%d;0H\n\033[2JFailure.\n", ws.ws_row + 1); 
                    return EXIT_SUCCESS; 
                }

                // It was a safe click, so count the bombs nearby.  
                // Iterate through the rows above to the one below, and columns left to right.  
                int bombs_nearby = 0; 
                for(int y = cursor_y - 1; y < cursor_y + 2; y++) {
                    for(int x = cursor_x - 1; x < cursor_x + 2; x++) {

                        // Make sure the y and x values don't go out of bounds from our array.  
                        if(y < 0 || ws.ws_row - 1 < y) {
                            break; 
                        } 
                        if(x < 0 || ws.ws_col - 1 < x) {
                            continue; 
                        }

                        // If there's a bomb, the buffer value is 1.  
                        // If not, it is 0.  
                        bombs_nearby += buffer[y][x]; 
                    }
                }
                printf("%d", bombs_nearby); 
                break; 
        }

        // Move the cursor to the position you've recorded.  
        printf("\033[%d;%dH", cursor_y + 1, cursor_x + 1); 
    }
    
    return EXIT_SUCCESS; 
}