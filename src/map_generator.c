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
 * @version 1.0.0 - 2020-12-05
 */

/**
 * @file map_generator.c
 *
 * Functions to generate the map of the city.
 */

#include <stdio.h>
#include <stdlib.h>

#include "map_generator.h"

void load_map(city_t *city)
{
    FILE *fp;
    
    char buffer[100];
    
    int col;
    int row;

    int indexes_taken[CITY_WIDTH][CITY_HEIGHT];
    
    if ((fp = fopen(MAP_URL, "r")) == NULL) {
        perror("No map file\n");
        exit(EXIT_FAILURE);
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
            init_fixed_tiles(city, indexes_taken, buffer, row);
        }
        else {        
            init_random_tiles(city, indexes_taken, buffer);
        }
        
        row++;
    }
    
    fclose(fp);
    
    replace_unitialized_tiles_with_wasteland(city, indexes_taken);
}

void init_fixed_tiles(city_t *city, int indexes_taken[CITY_WIDTH][CITY_HEIGHT], char* buffer,
                      int row) {
    double contamination;
    int col;
    
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
            perror("Error while reading the map\n");
            exit(EXIT_FAILURE);
        }
    }
}

void init_random_tiles(city_t *city, int indexes_taken[CITY_WIDTH][CITY_HEIGHT], char* buffer) {
    char building_type;
    int buildings_nb;
    int building;

    int col;
    int row;
    
    if (!sscanf(buffer, "%c %d", &building_type, &buildings_nb)) {
        perror("Error while reading the map parameters\n");
        exit(EXIT_FAILURE);
    }
    
    for (building = 0; building < buildings_nb; building++) {
        if (all_tile_indexes_are_taken(indexes_taken)) {
            perror("Too many buildings on the map\n");
            exit(EXIT_FAILURE);
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
            perror("Error while reading the map\n");
            exit(EXIT_FAILURE);
        }
    }
}

void replace_unitialized_tiles_with_wasteland(city_t *city,
                                              int indexes_taken[CITY_WIDTH][CITY_HEIGHT]) {
    double contamination;
    int col;
    int row;
    
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
