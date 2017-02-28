//
// Created by Francesco Arrigo
//


#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 100
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "printer.h"
#include "structures.h"
#include "algorithms.h"

int main(int argc, const char * argv[]) {
    clock_t tic = clock();

    unsigned int seed = (unsigned int)time(NULL);

    if (argc > 1) {
        char line[MAX_LINE_SIZE];
        size_t number = 0;
        // coordinates initializations
        coord *coordinates = malloc(sizeof(coord));
        if (!(coordinates)) {
            free(coordinates);
            return 1;
        }

        // check if file exists
        if (access(argv[1], F_OK) != -1) {
            FILE *tsp = fopen(argv[1], "r");
            if (!tsp) {
                printf("There was an error opening the file!\n");
                free(coordinates);
                return 1;
            }
            while (fgets(line, MAX_LINE_SIZE, tsp)) {

                SPACE_REMOVED:

                // check is first character of the line is a number
                if (isdigit(line[0])) {
                    char *token = strtok(line, " ");
                    token = strtok(NULL, " ");
                    number++;
                    coordinates = realloc(coordinates, number * sizeof(coord));
                    coordinates[number - 1].x = strtod(token, NULL);
                    token = strtok(NULL, " ");
                    coordinates[number - 1].y = strtod(token, NULL);
                    coordinates[number - 1].visited = 0;
                }
                    // if it's a space remove it and go back to the if
                else if (line[0] == ' ') {
                    int i = 0;
                    for (; i < strlen(line) - 1; i++) {
                        line[i] = line[i + 1];
                    }
                    line[i] = '\0';
                    goto SPACE_REMOVED;
                }
            }
            fclose(tsp);

            if (argv[2]) {
                seed = (unsigned int) strtol(argv[2], NULL, 10);
            }

            // set seed for random
            srand(seed);

            int start = rand() % number;

            // set up the matrix and fill it with 0s
            int distance = 0;
            int **map;
            map = calloc((number - 1), sizeof(int *));
            if (!(map)) {
                printf("Not enough memory!\n");
                free(map);
                free(coordinates);
                return 1;
            }
            for (int i = 0; i < number; i++) {
                map[i] = calloc(number, sizeof(int));
                if (!(map[i])) {
                    printf("Not enough memory!\n");
                    while (--i >= 0) {
                        free(map[i]);
                    }
                    free(map);
                    free(coordinates);
                    return 1;
                }
            }

            // create the result path array
            int *path_result = calloc(number - 1, sizeof(int));
            if (!(path_result)) {
                free(path_result);
                goto FREE_VARIABLES;
            }

            int *best = calloc(number - 1, sizeof(int));

            // compute the distance between all nodes
            for (int i = 0; i < number; i++) {
                for (int j = 0; j < number; j++) {
                    // avoid calculating distances twice and from a city to itself
                    if (i != j && map[i][j] == 0) {
                        distance = round(sqrt(pow(coordinates[i].x - coordinates[j].x, 2) + pow(coordinates[i].y - coordinates[j].y, 2)));
                        map[i][j] = distance;
                        map[j][i] = distance;
                    }
                }
            }

            // print the map
//            print_map(map, number, coordinates);

            int length;

            path_result = get_solution(map, number, coordinates, &length, start);

            simulated_annealing(map, path_result, &length, number);

            clock_t toc = clock();

            double time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;

            printf("Total time: %f minutes, seed: %d\n", time_spent / 60, seed);
            print_path(number,path_result, length);

            // FREE VARIABLES
            FREE_VARIABLES:
            for (int i = 0; i < number; i++) {
                free(map[i]);
            }
            free(map);
            free(coordinates);

        } else {
            printf("The selected file doesn't exist or it' not in the executable's directory\n");
        }
    } else {
        printf("File name needed \n");
        return 1;
    }
    return 0;
}

//h