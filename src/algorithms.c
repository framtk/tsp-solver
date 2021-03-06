//
// Created by Francesco Arrigo
//

#include "../include/algorithms.h"
#include "../include/structures.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int calculate_tour(int **map, path *course){
    int tourDistance = 0;
    for (int i = 0; i < course->city_number - 1; i++){
        tourDistance += map[course->path_result[i]][course->path_result[i + 1]];
    }
    tourDistance += map[course->path_result[0]][course->path_result[course->city_number - 1]];
    return tourDistance;
}

void nearest_neighbor(int **map, coord *coordinates, path *course){
    printf("running nearest neighbor\n");

    int start = (int) (rand() % course->city_number);

    course->length = 0;

    int save, save2, first, minDistance;

    save = save2 = first = start;
    minDistance = INT_MAX;
    for (int p = 0; p < course->city_number; p++){
        coordinates[p].visited = 0;
    }

    for (int i = 0; i < course->city_number - 1; i++) {
        if (map[save][i] != 0 && map[save][i] < minDistance) {
            minDistance = map[save][i];
            save2 = i;
        }
    }

    coordinates[first].visited = 1;
    course->path_result[0] = first;

    for (size_t i = 0; i < course->city_number - 1; i++) {
        minDistance = INT_MAX;
        for (int j = 0; j < course->city_number; j++) {
            if (save2 != j && map[save2][j] < minDistance && !(coordinates[j].visited)) {
                minDistance = map[save2][j];
                save = j;
            }
        }
        coordinates[save2].visited = 1;
        course->path_result[i + 1] = save2;
        course->path_result[i + 1] = save2;
        save2 = save;
    }

    course->length = calculate_tour(map,course);
}

void two_opt(int **map, path *temp){

    int min_distance = temp->length;
    int runs = 20;
    int check;
    int first, second, third, fourth;

    while(runs > 1) {
        check = 1;
        for (int i = 1; i < temp->city_number; ++i) {
            for (int j = i + 1; j < temp->city_number - 1; ++j) {

                first = map[temp->path_result[i - 1]][temp->path_result[j]];
                second = map[temp->path_result[i]][temp->path_result[j + 1]];
                third = map[temp->path_result[i - 1]][temp->path_result[i]];
                fourth = map[temp->path_result[j]][temp->path_result[j + 1]];

                //only check moves that will reduce distance
                if ((first + second) < (third + fourth)) {
                    swap(temp->path_result, i, j);
                    min_distance = calculate_tour(map, temp);
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

void swap(int *path_result, int i, int j) {
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

void simulated_annealing(int **map, path *course){

    // values may be suboptimal
    double degrees = 10000;
    double cooling = 0.996;

    path temp;
    temp.city_number = course->city_number;
    temp.path_result = calloc(course->city_number, sizeof(int));
    if (!(temp.path_result)){
        goto END;
    }

    double accept;
    double test;
    printf("running simulated annealing\n");

    while (degrees > 1){
        memcpy(temp.path_result, course->path_result, course->city_number * sizeof(int));

        size_t first_city = rand() % course->city_number;
        size_t second_city;
        do
            second_city = rand() % course->city_number;
        while (second_city == first_city);

        int saved_city = temp.path_result[first_city];
        temp.path_result[first_city] = temp.path_result[second_city];
        temp.path_result[second_city] = saved_city;

        two_opt(map, &temp);

        temp.length = calculate_tour(map, &temp);

        accept = rand() / (double) RAND_MAX;
        test = accept_solution(course->length, temp.length, degrees);
        if (accept < test){
            course->length = temp.length;
            memcpy(course->path_result, temp.path_result, course->city_number * sizeof(int));
        }

        if (temp.length < course->length){
            memcpy(course->path_result, temp.path_result, course->city_number * sizeof(int));
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