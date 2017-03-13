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
#include <limits.h>
#include "printer.h"
#include "structures.h"
#include "algorithms.h"

void get_solution(int **map, coord *coordinates, path *course){
    nearest_neighbor(map, coordinates, course);
    simulated_annealing(map, course);
}

int main(int argc, char *argv[]) {

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
                if (optopt)
                    fprintf (stderr, "There was an error with argument -%c\n", optopt);
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
                // check is first character of the line is a number
                if (isdigit(line[0])) {
                    char *token = strtok(line, " ");
                    token = strtok(NULL, " ");
                    current_path.city_number++;
                    coordinates = realloc(coordinates,
                                          current_path.city_number *
                                          sizeof(coord));
                    coordinates[current_path.city_number - 1].x = strtod(token,
                                                                         NULL);
                    token = strtok(NULL, " ");
                    coordinates[current_path.city_number - 1].y = strtod(token,
                                                                         NULL);
                    coordinates[current_path.city_number - 1].visited = 0;
                }
            }
            fclose(tsp);

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

            for (int i = 0; i < current_path.city_number; i++){
                current_path.path_result[i] = i;
            }

            path best;
            best.path_result = calloc(current_path.city_number, sizeof(int));
            best.length = INT_MAX;
            best.city_number = current_path.city_number;

            // compute the distance between all nodes
            for (int i = 0; i < current_path.city_number; i++) {
                for (int j = 0; j < current_path.city_number; j++) {
                    // avoid calculating distances twice and from a city to itself
                    if (i != j && map[i][j] == 0) {
                        distance = lround(
                                sqrt(pow(coordinates[i].x - coordinates[j].x,
                                         2) +
                                     pow(coordinates[i].y - coordinates[j].y,
                                         2)));
                        map[i][j] = (int) distance;
                        map[j][i] = (int) distance;
                    }
                }
            }

            current_path.length = calculate_tour(map,&current_path);

            // prints the map for debugging purposes
//            print_map(map, current_path.city_number, coordinates);

            double time_spent = 0;

            while (rep > 0 && known_solution < best.length) {
                printf("rep left %d\n",rep);

                srand(seed);

                current_path.seed = seed;

                clock_t tic = clock();

                get_solution(map, coordinates, &current_path);

                clock_t toc = clock();

                time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;

                current_path.time = time_spent;

                if (current_path.length < best.length) {
                    best.length = current_path.length;
                    best.seed = current_path.seed;
                    best.time = current_path.time;
                    memcpy(best.path_result, current_path.path_result, best.city_number * sizeof(int));
                }
                printf("length: %d, seed: %u\n\n",current_path.length, current_path.seed);
                rep--;
                seed = (unsigned int) time(NULL);

                // ensure that the seeds are different among reps if running time < 1 sec
                while (current_path.seed == seed || seed < current_path.seed)
                    seed++;
            }

            if (rep)
                printf("Optimal path found!\n");
            else
                printf("reached repetition number, best path found:\n");

                print_path(&best);

            // FREE VARIABLES
            FREE_VARIABLES:
            for (int i = 0; i < current_path.city_number; i++) {
                free(map[i]);
            }
            free(map);
            free(coordinates);
            free(best.path_result);

        } else {
            printf("The selected file doesn't exist or it' not in the executable's directory\n");
            return 1;
        }
    return 0;
}

//h