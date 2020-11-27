#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "color_printer/color_printer.h"
#include "color_printer/fade_printer.h"

#define COLUMN_NUMBER 34

int main(int argc, char** argv) {

    srand(time(NULL)); 
    color_printer_t* color_printers[COLUMN_NUMBER]; 
    for(int i = 0; i < COLUMN_NUMBER; i++) {
        color_printers[i] = color_printer_new(); 
        color_printer_set_color(color_printers[i], rand() % 256, rand() % 256, rand() % 256); 
    }

    fade_printer_t* fade_printers[COLUMN_NUMBER]; 
    for(int i = 0; i < COLUMN_NUMBER; i++) {
        fade_printers[i] = fade_printer_new(); 
        fade_printer_set_fade_rate(fade_printers[i], 10); 
        color_printer_set_color((color_printer_t*)fade_printers[i], rand() % 256, rand() % 256, rand() % 256); 
    }

    for(;;) {
        for(int i = 0; i < COLUMN_NUMBER; i++) {
            color_printer_print(color_printers[i]); 
        }
        for(int i = 0; i < COLUMN_NUMBER; i++) {
            fade_printer_fade(fade_printers[i]); 
            color_printer_print((color_printer_t*)fade_printers[i]); 
        }
        printf("\033[0m\n"); 
        fflush(stdout); 
        sleep(1); 
    }


    printf("\n"); 

    return EXIT_SUCCESS; 
}