//
// Created by Francesco Arrigo
//


#ifndef TSP_ALGORITHMS_H
#define TSP_ALGORITHMS_H

#include "printer.h"
#include <stdlib.h>

int* get_solution(int** map, size_t number, coord *coordinates, int* length, int start);

int* nearest_neighbor(int** map, size_t number, coord* coordinates, int* length, int start);

void two_opt(int** map, size_t number, int* path, int* length);

int calculate_tour(int** map, size_t number, int*path);

void swap(int* path, int i, int j);

void simulated_annealing(int** map, int *path, int* length, size_t number);

double accept_solution(int current_result, int result, double degrees);

#endif //TSP_ALGORITHMS_H
