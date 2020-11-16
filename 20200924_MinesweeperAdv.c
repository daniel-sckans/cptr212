#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <termios.h>

// Cells that have already been checked.  
#define CHECKED -2

// Bombs will be recorded as -1.  
#define BOMB -1

// These are called "forward declarations".  
// The actual definitions of the functions (what they do) are at the bottom of the page.  
// They're up here so they can be used before they're used in the code; in this case, it makes our code cleaner.  
char get_char_raw(); 
void clear_empties(int cursor_y, int cursor_x, struct winsize ws, int** bomb_map); 

int main(int argc, char* argv[argc + 1]) {

    // See previous samples.  
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // Seed the random function.  
    srand(time(NULL)); 

    // We'd normally store the bomb map like this: 
    // int bomb_map[ws.ws_row][ws.ws_col]; 
    // for(int y = 0; y < ws.ws_row; y++) {
    //     for(int x = 0; x < ws.ws_col; x++) {
    //         bomb_map[y][x] = 0; 
    //     }
    // }

    // This time, just for fun, we'll create a pointer to a pointer.  
    // We'll allocate memory at the second address to hold pointers.  
    // Then, we'll allocate memory to hold our bomb map (made of ints).  
    // Lastly, we'll initialize the value to 0.  
    int** bomb_map = (int**)malloc(ws.ws_row * sizeof(int*)); 
    for(int y = 0; y < ws.ws_row; y++) {
        bomb_map[y] = (int*)malloc(ws.ws_col * sizeof(int)); 
        for(int x = 0; x < ws.ws_col; x++) {
            bomb_map[y][x] = 0; 
        }
    }

    // Instead of checking how close we are to a bomb when a key is pressed, we'll just check once in the beginning.  
    // Our bomb map will record how far away each location is from a bomb.  
    for(int y = 0; y < ws.ws_row; y++) {
        for(int x = 0; x < ws.ws_col; x++) {
            printf("."); 
            
            // If the random number isn't a bomb, do nothing.  
            int is_bomb = rand() % 10 == 0; 
            if(!is_bomb) {
                continue; 
            }

            // If it is, we'll record it as the number -1.  
            // Then, we'll add increment all the counters nearby.  
            bomb_map[y][x] = BOMB; 
            for(int local_y = y - 1; local_y < y + 2; local_y++) {
                for(int local_x = x - 1; local_x < x + 2 && -1 < local_y && local_y < ws.ws_row; local_x++) {
                    if(local_x < 0 || ws.ws_col <= local_x) {
                        continue; 
                    }
                    if(bomb_map[local_y][local_x] != BOMB) {
                        bomb_map[local_y][local_x]++; 
                    }
                }
            }
        }
    }

    // Go to the top left.  
    printf("\033[H"); 

    // For input, see previous samples.  
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

                // -2 will record when a cell was already checked.  
                if(bomb_map[cursor_y][cursor_x] == CHECKED) {
                    break; 
                }

                // Game over.  
                if(bomb_map[cursor_y][cursor_x] == BOMB) {
                    printf("\033[%d;0H\n\033[48;2;255;0;0mFailure.\n", ws.ws_row + 1); 
                    return EXIT_SUCCESS; 
                }

                // Print the number of bombs nearby.  
                printf("\033[%d;%dH%d", cursor_y + 1, cursor_x + 1, bomb_map[cursor_y][cursor_x]); 

                // Clicked on an empty, so clear multiple cells.  
                if(bomb_map[cursor_y][cursor_x] == 0) {
                    clear_empties(cursor_y, cursor_x, ws, bomb_map); 
                    break; 
                } 

                // It's been checked, so the cell is set to -2.  
                bomb_map[cursor_y][cursor_x] = CHECKED; 
                break; 
            case 'r': 

                // The user marked a bomb; record it as -2 (already checked).  
                if(bomb_map[cursor_y][cursor_x] == BOMB) {
                    bomb_map[cursor_y][cursor_x] = CHECKED; 
                    printf("\033[48;2;255;0;0m \033[0m"); 
                    fflush(stdout); 

                    // If there are no bombs left, victory.  
                    int game_won = 1; 
                    for(int y = 0; y < ws.ws_row; y++) {
                        for(int x = 0; x < ws.ws_col; x++) {
                            if(bomb_map[y][x] == BOMB) {
                                game_won = 0; 
                            }
                        }
                    }
                    if(game_won) {
                        printf("\033[%d;0H\n\033[48;2;255;0;255mVictory.\033[0m", ws.ws_row + 1); 
                        return EXIT_SUCCESS; 
                    }
                }
        }
        printf("\033[%d;%dH", cursor_y + 1, cursor_x + 1); 
    }
    
    return EXIT_SUCCESS; 
}

// Recursive function to check connected empty spaces.  
void clear_empties(int cursor_y, int cursor_x, struct winsize ws, int** bomb_map) {
    bomb_map[cursor_y][cursor_x] = CHECKED; 

    // Iterate through the adjacent spaces.  
    for(int local_y = cursor_y - 1; local_y < cursor_y + 2; local_y++) {
        for(int local_x = cursor_x - 1; local_x < cursor_x + 2 && -1 < local_y && local_y < ws.ws_row; local_x++) {
            if(local_x < 0 || ws.ws_col <= local_x || bomb_map[local_y][local_x] == CHECKED) {
                continue; 
            }

            // Print the number of nearby bombs.  
            // If there are 0, check all the spaces around it.  
            printf("\033[%d;%dH%d", local_y + 1, local_x + 1, bomb_map[local_y][local_x]); 
            if(bomb_map[local_y][local_x] == 0) {
                clear_empties(local_y, local_x, ws, bomb_map); 
            }
        }
    }
}

// See previous samples.  
char get_char_raw() {
    struct termios canonical_term, raw_term; 
    tcgetattr(STDOUT_FILENO, &canonical_term); 
    raw_term = canonical_term; 
    raw_term.c_lflag &= ~ICANON; 
    raw_term.c_lflag &= ~ECHO; 
    tcsetattr(TCSANOW, STDOUT_FILENO, &raw_term); 
    char c = getchar(); 
    tcsetattr(TCSANOW, STDOUT_FILENO, &canonical_term); 
    return c; 
}