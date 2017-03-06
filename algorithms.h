//
// Created by Francesco Arrigo
//


#ifndef TSP_ALGORITHMS_H
#define TSP_ALGORITHMS_H

#include "structures.h"
#include <stdlib.h>

void get_solution(int** map, size_t number, coord *coordinates, path *course, int start);

int* nearest_neighbor(int** map, coord* coordinates, path *course, int start);

void two_opt(int** map, path *course);

int calculate_tour(int** map, size_t number, int*path);

void swap(int* path, int i, int j);

void simulated_annealing(int** map, path *course, size_t number);

double accept_solution(int current_result, int result, double degrees);

#endif //TSP_ALGORITHMS_H
