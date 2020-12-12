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
 * @author Jérémy Poullain <jeremy.poullain@ecole.ensicaen.fr>
 * @author Guillaume Revel <guillaume.revel@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-12-12
 */

/**
 * @file map_generator.c
 *
 * Functions to generate the map of the city.
 */

#include <stdio.h>
#include <stdlib.h>

#include "map_generator.h"
#include "util.h"

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

    init_city(city);

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

    contaminate_some_wastelands(city);
}

void init_city(city_t *city)
{
    city->wasteland_tiles_nb = 0;
    city->house_tiles_nb = 0;
    city->hospital_tiles_nb = 0;
    city->firestation_tiles_nb = 0;
}

void init_fixed_tiles(city_t *city, int indexes_taken[CITY_WIDTH][CITY_HEIGHT], char* buffer,
                      int row)
{
    int col;
    
    for (col = 0; col < CITY_WIDTH; col++) {
        switch (buffer[col]) {
        case 'F' :
            city->map[col][row] = init_tile_firestation(col, row);
            indexes_taken[col][row] = 1;
            city->firestation_tiles_nb++;
            break;
        case 'H' :
            city->map[col][row] = init_tile_hospital(col, row);
            indexes_taken[col][row] = 1;
            city->hospital_tiles_nb++;
            break;
        case 'O' :
            city->map[col][row] = init_tile_house(col, row);
            indexes_taken[col][row] = 1;
            city->house_tiles_nb++;
            break;
        case 'W' :
            city->map[col][row] = init_tile_wasteland(col, row);
            indexes_taken[col][row] = 1;
            city->wasteland_tiles_nb++;
            break;
        case 'X' : break;
        default :
            perror("Error while reading the map\n");
            exit(EXIT_FAILURE);
        }
    }
}

void init_random_tiles(city_t *city, int indexes_taken[CITY_WIDTH][CITY_HEIGHT], char* buffer)
{
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
            city->firestation_tiles_nb++;
            break;
        case 'H' :
            city->map[col][row] = init_tile_hospital(col, row);
            city->hospital_tiles_nb++;
            break;
        case 'O' :
            city->map[col][row] = init_tile_house(col, row);
            city->house_tiles_nb++;
            break;
        default :
            perror("Error while reading the map\n");
            exit(EXIT_FAILURE);
        }

        indexes_taken[col][row] = 1;
    }
}

void replace_unitialized_tiles_with_wasteland(city_t *city,
                                              int indexes_taken[CITY_WIDTH][CITY_HEIGHT])
{
    int col;
    int row;
    
    if (all_tile_indexes_are_taken(indexes_taken)) {
        return;
    }
    
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            if (!indexes_taken[col][row]) {
                city->map[col][row] = init_tile_wasteland(col, row);
                city->wasteland_tiles_nb++;
            }
        }
    }
}

void contaminate_some_wastelands(city_t *city)
{
    tile_t **wastelands = malloc(city->wasteland_tiles_nb*sizeof(tile_t *));

    double contamination;
    
    int col;
    int row;
    int i;
    int j;

    int clean_wastelands_nb;
    int contamined_wastelands_nb;
    int contamination_index;
    
    clean_wastelands_nb = 0;
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            if (city->map[col][row].type == WASTELAND) {
                wastelands[clean_wastelands_nb++] = &(city->map[col][row]);
            }
        }
    }

    contamined_wastelands_nb = (int)(clean_wastelands_nb
                                     * WASTELAND_PERCENT_CONTAMINED_AT_BEGINNING);

    for (i = 0; i < contamined_wastelands_nb; i++) {
        contamination = generate_random_wasteland_contamination_level();

        contamination_index = generate_random_index(clean_wastelands_nb);
        wastelands[contamination_index]->contamination = contamination;
        
        for (j = contamination_index; j < clean_wastelands_nb - 1; j++) {
            wastelands[j] = wastelands[j + 1];
        }
        
        clean_wastelands_nb--;
    }
    
    free(wastelands);
}

double generate_random_wasteland_contamination_level()
{
    return generate_random_percentage_in_interval(MIN_WASTELAND_CONTAMINATION_AT_BEGINNING,
                                                  MAX_WASTELAND_CONTAMINATION_AT_BEGINNING);
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
