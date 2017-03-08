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
#include <unistd.h>
#include "printer.h"
#include "structures.h"
#include "algorithms.h"

void get_solution(int** map, coord *coordinates, path *course, int start){
    nearest_neighbor(map, coordinates, course, start);
    simulated_annealing(map, course);
}

int main(int argc, char *argv[]) {
    clock_t tic = clock();

    int rep = 1;
    char *filename = NULL;
    unsigned int seed = (unsigned int)time(NULL);
    int filename_flag = 1;
    int arg;

    long known_solution;

    while ((arg = getopt (argc, argv, "f:s:r:")) != -1)
        switch (arg)
        {
            case 'f':
                filename = optarg;
                filename_flag = 0;
                break;
            case 's':
                seed = (unsigned int)strtol(optarg, NULL, 10);
                break;
            case 'r':
                rep = (int)strtol(optarg, NULL, 10);
                break;
            case '?':
                if (optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 's')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 'r')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort ();
        }
    if (filename_flag){
        fprintf (stderr, "Filename containing tsp needed, use -f filepath.\n");
        return 1;
    }

        path current_path;

        char line[MAX_LINE_SIZE];
        current_path.city_number = 0;
        // coordinates initializations
        coord *coordinates = malloc(sizeof(coord));
        if (!(coordinates)) {
            free(coordinates);
            return 1;
        }

        // check if file exists
        if (access(filename, F_OK) != -1) {
            FILE *tsp = fopen(filename, "r");
            if (!tsp) {
                printf("There was an error opening the file!\n");
                free(coordinates);
                return 1;
            }

            fgets(line, MAX_LINE_SIZE, tsp);
            known_solution = strtol(line, NULL, 10);

            while (fgets(line, MAX_LINE_SIZE, tsp)) {

                SPACE_REMOVED:

                // check is first character of the line is a number
                if (isdigit(line[0])) {
                    char *token = strtok(line, " ");
                    token = strtok(NULL, " ");
                    current_path.city_number++;
                    coordinates = realloc(coordinates, current_path.city_number * sizeof(coord));
                    coordinates[current_path.city_number - 1].x = strtod(token, NULL);
                    token = strtok(NULL, " ");
                    coordinates[current_path.city_number - 1].y = strtod(token, NULL);
                    coordinates[current_path.city_number - 1].visited = 0;
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

            // set seed for random
            srand(seed);

            int start = (int)(rand() % current_path.city_number);

            // set up the matrix and fill it with 0s
            long distance = 0;
            int **map;
            map = calloc((current_path.city_number), sizeof(int *));
            if (!(map)) {
                printf("Not enough memory!\n");
                free(map);
                free(coordinates);
                return 1;
            }
            for (int i = 0; i < current_path.city_number; i++) {
                map[i] = calloc(current_path.city_number, sizeof(int));
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

            current_path.path_result = calloc(current_path.city_number, sizeof(int));
            if (!(current_path.path_result)) {
                free(current_path.path_result);
                goto FREE_VARIABLES;
            }

            int *best = calloc(current_path.city_number, sizeof(int));

            // compute the distance between all nodes
            for (int i = 0; i < current_path.city_number; i++) {
                for (int j = 0; j < current_path.city_number; j++) {
                    // avoid calculating distances twice and from a city to itself
                    if (i != j && map[i][j] == 0) {
                        distance = lround(sqrt(pow(coordinates[i].x - coordinates[j].x, 2) + pow(coordinates[i].y - coordinates[j].y, 2)));
                        map[i][j] = (int)distance;
                        map[j][i] = (int)distance;
                    }
                }
            }

            // prints the map for debugging purposes
//            print_map(map, current_path.city_number, coordinates);

            get_solution(map, coordinates, &current_path, start);

            clock_t toc = clock();

            double time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;

            printf("Total time: %f minutes, seed: %d\n", time_spent / 60, seed);
            print_path(current_path.city_number, current_path.path_result, current_path.length);

            // FREE VARIABLES
            FREE_VARIABLES:
            for (int i = 0; i < current_path.city_number; i++) {
                free(map[i]);
            }
            free(map);
            free(coordinates);
            free(best);

        } else {
            printf("The selected file doesn't exist or it' not in the executable's directory\n");
            return 1;
        }
    return 0;
}

//h