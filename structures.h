//
// Created by Francesco Arrigo on 28/02/17.
//

#ifndef TSP_STRUCTURES_H
#define TSP_STRUCTURES_H

#include <stdlib.h>

typedef struct coordinate{
    double x;
    double y;
    int visited;
}coord;

typedef struct course{
    int *path_result;
    int length;
    size_t city_number;
    unsigned int seed;
    double time;
}path;

#endif //TSP_STRUCTURES_H
