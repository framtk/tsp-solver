//
// Created by Francesco Arrigo on 28/02/17.
//

#ifndef TSP_STRUCTURES_H
#define TSP_STRUCTURES_H

typedef struct coordinate{
    double x;
    double y;
    int visited;
}coord;

typedef struct course{
    int *path_result;
    int length;
}path;

#endif //TSP_STRUCTURES_H
