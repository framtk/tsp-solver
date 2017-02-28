//
// Created by Francesco Arrigo
//

#include "algorithms.h"
#include "structures.h"
#include <limits.h>
#include <string.h>
#include <math.h>

int calculate_tour(int** map, size_t number, int*path){
    int tourDistance = 0;
    for (int i = 0; i < number - 1; i++){
        tourDistance += map[path[i]][path[i + 1]];
    }
    tourDistance += map[path[0]][path[number - 1]];
    return tourDistance;
}

int* nearest_neighbor(int** map, size_t number, coord *coordinates, int* length, int start){
    *length = INT_MAX;
    int* temp = calloc(number - 1, sizeof(int));
    if (!(temp)){
        goto SKIP_NN;
    }

    int* solution = calloc(number - 1, sizeof(int));
    if (!(solution)){
        free(solution);
        goto SKIP_NN;
    }

    int save, save2, first, minDistance;

        save = save2 = first = start;
        minDistance = INT_MAX;
        for (int p = 0; p < number; p++){
            coordinates[p].visited = 0;
        }

        // end reset

        for (int i = 0; i < number - 1; i++) {
            if (map[save][i] != 0 && map[save][i] < minDistance) {
                minDistance = map[save][i];
                save2 = i;
            }
        }

        int tour = 0;
        coordinates[first].visited = 1;
        temp[0] = first;

        for (size_t i = 0; i < number - 1; i++) {
            minDistance = INT_MAX;
            for (int j = 0; j < number; j++) {
                if (save2 != j && map[save2][j] < minDistance && !(coordinates[j].visited)) {
                    minDistance = map[save2][j];
                    save = j;
                }
            }
            coordinates[save2].visited = 1;
            temp[i + 1] = save2;
            temp[i + 1] = save2;
            save2 = save;
        }

        // apply 2-opt
        int temp_len = *length;
        two_opt(map,number,temp,&temp_len);
        tour = calculate_tour(map, number, temp);

        // save if better than previous one
        if (tour < *length) {
            *length = tour;
            memcpy(solution, temp, number * sizeof(int));
        }

    free(temp);
    return solution;
    SKIP_NN:
    free(temp);
    return 0;
}

void two_opt(int** map, size_t number, int* path, int* length){

    int min_distance = *length;
    int runs = 20;
    int check;

    while(runs > 1) {
        check = 1;
        for (int i = 1; i < number; ++i) {
            for (int j = i + 1; j < number - 1; ++j) {

                //only check moves that will reduce distance
                if ((map[path[i - 1]][path[j]] + map[path[i]][path[j + 1]]) < (map[path[i - 1]][path[i]] + map[path[j]][path[j + 1]])) {
                    swap(path, i, j);
                    min_distance = calculate_tour(map, number, path);
                    check = 0;
                }
            }
        }
        runs--;
        if (check){
            runs = 0;
        }
    }

    *length = min_distance;
}

void swap(int* path, int i, int j) {
    size_t size = (size_t)j;
    int *temp = calloc(size, sizeof(int));
    if (!temp){
        goto END_SWAP;
    }
    int count = 0;

    //Reverse order
    for (int x = j; x >= i; --x)
    {
        temp[count] = path[x];
        count++;
    }

    count = 0;
    for (int x = i; x <= j; ++x)
    {
        path[x] = temp[count];
        ++count;
    }
    END_SWAP:
    free(temp);
}

void simulated_annealing(int** map, path *course, size_t number){

    // best degrees and cooling to assure < 3 min run time on all problems
    double degrees = 10000;
    double cooling = 0.996;

    path temp;

    temp.path_result = calloc(number - 1, sizeof(int));
    if (!(temp.path_result)){
        goto END;
    }

    double accept;
    double test;

    while (degrees > 1){
        memcpy(temp.path_result, course->path_result, number * sizeof(int));

        size_t first_city = rand() % number;
        size_t second_city;
        do
            second_city = rand() % number;
        while (second_city == first_city);

        int save_city = temp.path_result[first_city];
        temp.path_result[first_city] = temp.path_result[second_city];
        temp.path_result[second_city] = save_city;

        temp.length = calculate_tour(map, number, temp.path_result);

        two_opt(map, number, temp.path_result, &temp.length);

        accept = rand() / (double) RAND_MAX;
        test = accept_solution(course->length, temp.length, degrees);
        if (accept < test){
            course->length = temp.length;
            memcpy(course->path_result, temp.path_result, number * sizeof(int));
        }

        if (temp.length < course->length){
            memcpy(course->path_result, temp.path_result, number * sizeof(int));
            course->length = temp.length;
        }
        degrees = degrees * cooling;
    }

    END:
    free(temp.path_result);
}

double accept_solution(int current_result, int result, double degrees){
    return pow(M_E, (current_result - result) / degrees);
}

int* get_solution(int** map, size_t number, coord *coordinates, int* length, int start){
    return nearest_neighbor(map, number, coordinates, length, start);
}