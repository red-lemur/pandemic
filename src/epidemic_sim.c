/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin 
 * F-14050 Caen Cedex 
 * 
 * This file is owned by ENSICAEN.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr> [original author]
 * @author Jérémy Poullain <jeremy.poullain@ecole.ensicaen.fr>
 * @author Guillaume Revel <guillaume.revel@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-12-03
 */

/**
 * @file epidemic_sim.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <mqueue.h>

#include "epidemic_sim.h"

int create_shared_memory()
{
    int mem;
    
    mem = shm_open(SHARED_MEM, O_CREAT | O_RDWR, S_IRWXU);
    if (mem < 0) {
        perror("Error when calling shm_open()\n");
        exit(EXIT_FAILURE);
    }
    
    if (ftruncate(mem, sizeof(city_t)) < 0) {
        perror("Error when calling ftruncate()\n");
        exit(EXIT_FAILURE);
    }
    
    return mem;
}

void generate_city(city_t *city)
{
    load_map(city);

    // Générer citoyens ?
}

void load_map(city_t *city) // A DIVISER
{
    FILE *fp;
    
    char buffer[100];
    char building_type;
    
    int col;
    int row;

    int indexes_taken[CITY_WIDTH][CITY_HEIGHT];
    int buildings_nb;
    int building;
    
    double contamination;
    
    if ((fp = fopen(MAP_URL, "r")) == NULL) {
        printf("No map file\n");
        exit(-1);
    }

    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            indexes_taken[col][row] = 0;
        }   
    }
    
    row = 0;
    while (fscanf(fp, "%[^\n]\n", buffer) != EOF) {
        if (buffer[0] == '\n' || (buffer[0] == '/' && buffer[1] == '/')
            || buffer[0] == ' ' || buffer[0] == '\t') {
            continue;
        }
        
        if (row < CITY_HEIGHT) {
            for (col = 0; col < CITY_WIDTH; col++) {
                switch (buffer[col]) {
                case 'F' :
                    city->map[col][row] = init_tile_firestation(col, row);
                    indexes_taken[col][row] = 1;
                    break;
                case 'H' :
                    city->map[col][row] = init_tile_hospital(col, row);
                    indexes_taken[col][row] = 1;
                    break;
                case 'O' :
                    city->map[col][row] = init_tile_house(col, row);
                    indexes_taken[col][row] = 1;
                    break;
                case 'W' :
                    contamination = generate_random_wasteland_contamination_level();
                    city->map[col][row] = init_tile_wasteland(col, row, contamination);
                    indexes_taken[col][row] = 1;
                    break;
                case 'X' : break;
                default :
                    printf("Error while reading the map\n");
                    exit(-1);
                }
            }
        }
        else {        
            if (!sscanf(buffer, "%c %d", &building_type, &buildings_nb)) {
                printf("Error while reading the map parameters\n");
                exit(-1);
            }

            for (building = 0; building < buildings_nb; building++) {
                if (all_tile_indexes_are_taken(indexes_taken)) {
                    printf("Too many buildings on the map\n");
                    exit(-1);
                }
                
                do {
                    col = generate_random_index(CITY_WIDTH);
                    row = generate_random_index(CITY_HEIGHT);    
                } while (indexes_taken[col][row]);
                
                switch (building_type) {                
                case 'F' :
                    city->map[col][row] = init_tile_firestation(col, row);
                    indexes_taken[col][row] = 1;
                    break;
                case 'H' :
                    city->map[col][row] = init_tile_hospital(col, row);
                    indexes_taken[col][row] = 1;
                    break;
                case 'O' :
                    city->map[col][row] = init_tile_house(col, row);
                    indexes_taken[col][row] = 1;
                    break;
                default :
                    printf("Error while reading the map\n");
                    exit(-1);
                }
            }
        }

        row++;
    }

    fclose(fp);
    
    if (!all_tile_indexes_are_taken(indexes_taken)) {
        for (row = 0; row < CITY_HEIGHT; row++) {
            for (col = 0; col < CITY_HEIGHT; col++) {
                if (!indexes_taken[col][row]) {
                    contamination = generate_random_wasteland_contamination_level();
                    city->map[col][row] = init_tile_wasteland(col, row, contamination);
                }
            }   
        }
    }
}

double generate_random_wasteland_contamination_level()
{
    return rand()/(double)RAND_MAX * (MAX_WASTELAND_CONTAMINATION_AT_BEGINNING
                                      - MIN_WASTELAND_CONTAMINATION_AT_BEGINNING)
        + MIN_WASTELAND_CONTAMINATION_AT_BEGINNING;
}

int generate_random_index(int ind_max)
{
    return (int)(rand()/(double)RAND_MAX * ind_max);
}

int all_tile_indexes_are_taken(int indexes_taken[CITY_WIDTH][CITY_HEIGHT])
{
    int row;
    int col;
    
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            if (!indexes_taken[col][row]) {
                return 0;
            }
        }   
    }
    
    return 1;
}

/* mqd_t create_mqueue(); */

int main(void)
{
    int mem;
    /*mqd_t mqueue;*/

    city_t *city;
    
    srand(time(NULL));
    
    mem = create_shared_memory();
    /*mqueue = create_mqueue();*/

    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);
    generate_city(city);

    /* TEST DEBUG MAP */
    /*int row, col;
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            printf("%d", city->map[col][row].type);
        }
        printf("\n");
    }*/
    
    /* Début de la simulation */
    /* C'est à vous d'écrire là ... */
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    if (close(mem) < 0) {
        perror("Error when calling close(mem)\n");
    }
    if (shm_unlink(SHARED_MEM) < 0) {
        perror("Error when calling shm_unlink()\n");
    }
    exit(EXIT_SUCCESS);
}
