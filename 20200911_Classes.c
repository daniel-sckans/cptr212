#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

// Our runner struct to hold or encapsulate information about our object.  
// In this case we'll include position information ("x" and "y") and color information ("r", "g", and "b").  
typedef struct {
	int x; 
	int y; 
	int r; 
	int g; 
	int b; 
} runner; 

// This is some functionality we'd like to have our runner object to have.  
// If this was a random function, we'd just call it a function; because it is related to an object (the runner), we call it a method.  
// Because we want to be able to alter an object, we need to send in a pointer to a runner ("runner* e").  
// If we sent in a regular runner ("runner e"), the function would get a separate copy of it.  
void runner_maybe_move(runner* e) {
	if(rand() % 2 == 0) {
		e->x++; 
	}
}

int main(int argc, char* argv[argc + 1]) {

    // See previous samples for winsize.  
	struct winsize ws; 
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 

    // Print out a dot for each character in the terminal.  
	for(int i = 0; i < ws.ws_col * ws.ws_row; i++) {
		printf("."); 
	}

    // Here's we'll initialize our runners (notice each one is a pointer, "runner*").  
    // They'll be contained in an array called runners.  
    // We need to set aside some memory for each of them with malloc; "sizeof(runner)" makes sure we use the right size in memory.  
	runner* runners[ws.ws_row]; 
	for(int i = 0; i < ws.ws_row; i++) {
		runners[i] = malloc(sizeof(runner)); 
		runners[i]->x = 1; 
		runners[i]->y = i + 1; 
		runners[i]->r = i * 255 / ws.ws_row; 
		runners[i]->g = 250 - i * 255 / ws.ws_row; 
		runners[i]->b = 0; 
	}

    // We'll use rand in a minute, so we need to seed it ("srand") with the current time.  
	srand(time(NULL)); 

	while(1) {

        // For each runner, print its location ("%d;%dH"), color ("48;2;%d;%d;%dm"), a space (" "), then reset the color ("0m").  
        // These are all ANSI codes.  
		for(int i = 0; i < ws.ws_row; i++) {
			printf("\033[%d;%dH\033[48;2;%d;%d;%dm \033[0m", runners[i]->y, runners[i]->x, runners[i]->r, runners[i]->g, runners[i]->b); 
			runner_maybe_move(runners[i]); 
		}

        // Print the output and sleep for 0.2 seconds.  
		fflush(stdout); 
		usleep(200000); 
	}; 

	return EXIT_SUCCESS; 
}