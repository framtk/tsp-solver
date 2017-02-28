//
// Created by Francesco Arrigo
//


#include <stdio.h>
#include "printer.h"

void print_map(int** map, size_t number, coord* coordinates){
    for (int i = 0; i < number; i++){
        for (int j = 0; j < number; j++){
            printf("node [%d][%d] xi: %3.10f yi: %3.10f, xj: %3.10f yj: %3.10f.  distance: %d\n",i,j,coordinates[i].x,coordinates[i].y,coordinates[j].x,coordinates[j].y,map[i][j]);
        }
    }
}


void print_path(size_t number, int* path, int length){
    for (int i = 0; i < number; i++){
        printf("%d ", path[i] + 1);
    }
    printf("\ntotal length: %d\n",length);
}
