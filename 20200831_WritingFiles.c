#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

// Borrowed from 20200824_Files.c (see that file for comments).  
char get_char() {
    struct termios old_term, new_term; 
    tcgetattr(STDIN_FILENO, &old_term); 
    new_term = old_term; 
    new_term.c_lflag &= ~ICANON; 
    new_term.c_lflag &= ~ECHO; 
    tcsetattr(TCSANOW, STDIN_FILENO, &new_term); 
    char c = getchar(); 
    tcsetattr(TCSANOW, STDIN_FILENO, &old_term); 
    return c; 
}

int main(int argc, char* argv[argc + 1]) {

    // This section is taken directly from 20200826_FilesWithBuffer.c (see that file for comments).  
    struct winsize ws; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); 
    if(argc < 2) {
        fprintf(stdout, "Please include one argument for the file name.\n"); 
        return EXIT_FAILURE; 
    }
    FILE* txt_file = fopen(argv[1], "r"); 
    if(!txt_file) {
        fprintf(stdout, "No file.\n"); 
    }
    int line_count = 0; 
    char buf[ws.ws_col + 1]; 
    while(!feof(txt_file)) {
        fgets(buf, ws.ws_col + 1, txt_file); 
        line_count++; 
    }
    fseek(txt_file, 0, SEEK_SET); 
    char txt[line_count][ws.ws_col + 1]; 
    for(int y = 0; y < line_count; y++) {
        fgets(txt[y], ws.ws_col + 1, txt_file); 
    }
    fclose(txt_file); 

    // New code: saving files.  
    // General strategy: we'll start by replacing text in the buffer, and then we'll write the buffer to a new file.  

    // We can replace any character we'd like.  
    txt[4][0] = 'Z'; 

    // We can replace an entire line of text using the <string.h> function "strcpy", which copies a string.  
    strcpy(txt[7], "It was just the best of times, not the worst of times.\n"); 

    // Why do we have to use that function (instead of just copying a string)?  
    // This will not compile; we'll talk about the underlying reasons why on Wednesday.  
    // txt[8] = "This is a sample string."; 

    // We'll include some controls here to print the text to the terminal.  
    int text_offset = 0; 
    char c = '\0'; 
    do {
        for(int y = 0; y < ws.ws_row; y++) {
            printf("%s", txt[y + text_offset]); 
        }
    } while((c = get_char()) != 'q'); 

    // Now we'll save the file.  
    // First, we'll open a file using "w" ("write") mode.  
    // Then, we'll just print to it using fprintf(), which is just like printf except it takes a file as its first parameter.  
    FILE* out_file = fopen("assets/ATaleOfThreeCities.txt", "w"); 
    if(!out_file) {
        printf("Not able to creat an output file.\n"); 
        return EXIT_FAILURE; 
    }
    for(int y = 0; y < line_count; y++) {
        fprintf(out_file, "%s", txt[y]); 
    }
    fclose(out_file); 

    return EXIT_SUCCESS; 
}