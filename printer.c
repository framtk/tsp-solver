//
// Created by Francesco Arrigo
//


#include <stdio.h>
#include "printer.h"

void print_map(int **map, size_t city_number, coord *coordinates){
    for (int i = 0; i < city_number; i++){
        for (int j = 0; j < city_number; j++){
            printf("node [%d][%d] xi: %3.10f yi: %3.10f, xj: %3.10f yj: %3.10f.  distance: %d\n",i,j,coordinates[i].x,coordinates[i].y,coordinates[j].x,coordinates[j].y,map[i][j]);
        }
    }
}


void print_path(path *course){
    for (int i = 0; i < course->city_number; i++){
        printf("%d ", course->path_result[i] + 1);
    }
    printf("\ntotal length: %d, seed: %u\n\n",course->length,course->seed);
}
