#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define COLUMN_NUMBER 34

typedef struct color_printer_t color_printer_t; 
color_printer_t* color_printer_new(); 
void color_printer_set_color(); 
void color_printer_print(); 

typedef struct fade_printer_t fade_printer_t; 
fade_printer_t* fade_printer_new(); 
void fade_printer_set_fade_rate(); 
void fade_printer_fade(); 

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

typedef struct color_printer_t {
    int r; 
    int g; 
    int b; 
} color_printer_t; 

color_printer_t* color_printer_new() {
    color_printer_t* color_printer = malloc(sizeof(color_printer_t)); 
    color_printer->r = 0; 
    color_printer->g = 0; 
    color_printer->b = 0; 
    return color_printer; 
}

void color_printer_set_color(color_printer_t* color_printer, int r, int g, int b) {
    color_printer->r = r; 
    color_printer->g = g; 
    color_printer->b = b; 
}

void color_printer_print(color_printer_t* color_printer) {
    printf("\033[48;2;%d;%d;%dm ", color_printer->r, color_printer->g, color_printer->b); 
    printf("\033[0m"); 
}

typedef struct fade_printer_t {
    color_printer_t color_Printer; 
    int fade_rate; 
} fade_printer_t; 

fade_printer_t* fade_printer_new() {
    fade_printer_t* fade_printer = malloc(sizeof(fade_printer_t)); 
    fade_printer->color_Printer.r = 0; 
    fade_printer->color_Printer.g = 0; 
    fade_printer->color_Printer.b = 0; 
    fade_printer->fade_rate = 1; 
}

void fade_printer_set_fade_rate(fade_printer_t* fade_printer, int rate) {
    fade_printer->fade_rate = rate; 
}

void fade_printer_fade(fade_printer_t* fade_printer) {
    fade_printer->color_Printer.r -= fade_printer->fade_rate; 
    fade_printer->color_Printer.g -= fade_printer->fade_rate; 
    fade_printer->color_Printer.b -= fade_printer->fade_rate; 
    if(fade_printer->color_Printer.r < 0) {
        fade_printer->color_Printer.r = 0; 
    }
    if(fade_printer->color_Printer.g < 0) {
        fade_printer->color_Printer.g = 0; 
    }
    if(fade_printer->color_Printer.b < 0) {
        fade_printer->color_Printer.b = 0; 
    }
}