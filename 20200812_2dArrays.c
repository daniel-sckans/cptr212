#include <stdlib.h> // We're using this for EXIT_SUCCESS.  
#include <stdio.h> // For printf().
#include <unistd.h> // UNIX (technically POSIX).  
#include <sys/ioctl.h> // In-out control; for getting the terminal size.  

int main(int argc, char* argv[argc+1]) {

    // Declaring a two-dimensional array.  
    int a[2][3] = { {1, 2, 3}, {4, 5, 6} }; 
    
    // Getting the size of the window in a struct called "ws" of type winsize.  
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // Two-dimensional arrow of ints.  
    // The array dimensions match up to the terminal columns and rows.  
    // We declare the array, and then fill it using nested for-loops.  
    int t[ws.ws_col][ws.ws_row]; 
    for(int x = 0; x < ws.ws_col; x++) {
        for(int y = 0; y < ws.ws_row; y++) {
            t[x][y] = 1; 
        }
    }

    // Character version.  
    // Unlike the previous version, this goes one line at a time, from left to right (because the outer for-loop is the rows, and the inner for-loop is the columns).  
    // Characters are one letter and are defined with apostrophes on either side, i.e. 'e'.  
    char tc[ws.ws_row][ws.ws_col]; 
    for(int y = 0; y < ws.ws_row; y++) {
        for(int x = 0; x < ws.ws_col; x++) {
            tc[y][x] = 'a'; 

            // An example of changing one column of characters.  
            if(x == 3) {
                tc[y][x] = 'z'; 
            }
        }
    }

    // Print the array to the screen.  
    // This is almost identical to the previous section, and we don't need to store anything in an array if all we're doing is printing.  
    // %c prints a character (%d instead prints a number).  
    // usleep() sleeps for a number of microseconds (or one millionth of a second).  
    // Use fflush(stdout) to print all the output that's waiting in the stdout device (which is what shows up in your terminal).  
    for(int y = 0; y < ws.ws_row; y++) {
        for(int x = 0; x < ws.ws_col; x++) {
            fflush(stdout); 
            usleep(10000); 
            printf("%c", tc[y][x]); 
        }
    }

    return EXIT_SUCCESS; 
}