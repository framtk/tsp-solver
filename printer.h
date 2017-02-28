//
// Created by Francesco Arrigo
//


#ifndef TSP_PRINTER_H
#define TSP_PRINTER_H

#include <stdlib.h>

typedef struct coordinate{
    double x;
    double y;
    int visited;
}coord;

void print_map(int** map, size_t number, coord* coordinates);

void print_path(size_t number, int* path, int length);

#endif //TSP_PRINTER_H
