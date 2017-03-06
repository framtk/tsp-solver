//
// Created by Francesco Arrigo
//

#include "algorithms.h"
#include "structures.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int calculate_tour(int **map, size_t city_number, int *path){
    int tourDistance = 0;
    for (int i = 0; i < city_number - 1; i++){
        tourDistance += map[path[i]][path[i + 1]];
    }
    tourDistance += map[path[0]][path[city_number - 1]];
    return tourDistance;
}

// TODO
int *nearest_neighbor(int **map, coord *coordinates, path *course, int start){
    course->length = INT_MAX;
    path temp;
    temp.length = 0;
    temp.city_number = course->city_number;
    temp.path_result = calloc(course->city_number - 1, sizeof(int));
    if (!(temp.path_result)){
        goto SKIP_NN;
    }

    int save, save2, first, minDistance;

        save = save2 = first = start;
        minDistance = INT_MAX;
        for (int p = 0; p < course->city_number; p++){
            coordinates[p].visited = 0;
        }

        // end reset

        for (int i = 0; i < course->city_number - 1; i++) {
            if (map[save][i] != 0 && map[save][i] < minDistance) {
                minDistance = map[save][i];
                save2 = i;
            }
        }

        coordinates[first].visited = 1;
        temp.path_result[0] = first;

        for (size_t i = 0; i < course->city_number - 1; i++) {
            minDistance = INT_MAX;
            for (int j = 0; j < course->city_number; j++) {
                if (save2 != j && map[save2][j] < minDistance && !(coordinates[j].visited)) {
                    minDistance = map[save2][j];
                    save = j;
                }
            }
            coordinates[save2].visited = 1;
            temp.path_result[i + 1] = save2;
            temp.path_result[i + 1] = save2;
            save2 = save;
        }

        // apply 2-opt
        temp.length = course->length;
        two_opt(map, &temp);
        temp.length = calculate_tour(map, course->city_number, temp.path_result);

        // save if better than previous one
        if (temp.length < course->length) {
            course->length = temp.length;
            memcpy(course->path_result, temp.path_result, course->city_number * sizeof(int));
        }
    SKIP_NN:
    free(temp.path_result);
}

void two_opt(int** map, path *temp){

    int min_distance = temp->length;
    int runs = 20;
    int check;

    while(runs > 1) {
        check = 1;
        for (int i = 1; i < temp->city_number; ++i) {
            for (int j = i + 1; j < temp->city_number - 1; ++j) {

                //only check moves that will reduce distance
                if ((map[temp->path_result[i - 1]][temp->path_result[j]] +
                        map[temp->path_result[i]][temp->path_result[j + 1]]) < (map[temp->path_result[i - 1]][temp->path_result[i]] +
                        map[temp->path_result[j]][temp->path_result[j + 1]])) {
                    swap(temp->path_result, i, j);
                    min_distance = calculate_tour(map, temp->city_number, temp->path_result);
                    check = 0;
                }
            }
        }
        runs--;
        if (check){
            runs = 0;
        }
    }
    temp->length = min_distance;
}

void swap(int* path_result, int i, int j) {
    size_t size = (size_t)j;
    int *temp = calloc(size, sizeof(int));
    if (!temp){
        goto END_SWAP;
    }
    int count = 0;

    //Reverse order
    for (int x = j; x >= i; --x)
    {
        temp[count] = path_result[x];
        count++;
    }

    count = 0;
    for (int x = i; x <= j; ++x)
    {
        path_result[x] = temp[count];
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
    temp.city_number = course->city_number;
    temp.path_result = calloc(number - 1, sizeof(int));
    if (!(temp.path_result)){
        goto END;
    }

    double accept;
    double test;

    while (degrees > 1){
        printf("temperature: %f\n",degrees);
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

        two_opt(map, &temp);

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