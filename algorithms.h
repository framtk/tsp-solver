//
// Created by Francesco Arrigo
//


#ifndef TSP_ALGORITHMS_H
#define TSP_ALGORITHMS_H

#include "structures.h"
#include <stdlib.h>

void get_solution(int** map, coord *coordinates, path *course, int start);

void nearest_neighbor(int** map, coord* coordinates, path *course, int start);

void two_opt(int** map, path *course);

int calculate_tour(int** map, path *course);

void swap(int* path, int i, int j);

void simulated_annealing(int** map, path *course);

double accept_solution(int current_result, int result, double degrees);

#endif //TSP_ALGORITHMS_H
