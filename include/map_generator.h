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
 * @version 1.0.0 - 2020-12-08
 */

/**
 * @file map_generator.h
 *
 * Functions to generate the map of the city.
 */

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "city_sim.h"

/**
 * @brief Load the map of the city from a text file.
 * @param city City to load.
 */
void load_map(city_t *city);

/**
 * @brief Initialize the tiles of a row of the map of the city placed at a certain index.
 * @param city City whose map is initialized.
 * @param indexes_taken Array indicating the indexes already initialized.
 * @param buffer A row from the map text file.
 * @param row Index of the row of the map currently initialized.
 */
void init_fixed_tiles(city_t *city, int indexes_taken[CITY_WIDTH][CITY_HEIGHT], char* buffer,
                      int row);

/**
 * @brief Initialize the buildings of the map that have to be placed randomly.
 * @param city City whose map is initialized.
 * @param indexes_taken Array indicating the indexes already initialized.
 * @param buffer A row from the map text file.
 */
void init_random_tiles(city_t *city, int indexes_taken[CITY_WIDTH][CITY_HEIGHT], char* buffer);

/**
 * @brief Initialize the tiles that were not previously initialized with the WASTELAND type.
 * @param city City whose map is initialized.
 * @param indexes_taken Array indicating the indexes already initialized.
 */
void replace_unitialized_tiles_with_wasteland(city_t *city,
                                              int indexes_taken[CITY_WIDTH][CITY_HEIGHT]);

/**
 * @brief Generate a random contamination level to initiate a tile of WASTELAND type.
 * @return A contamination level between MIN and MAX_WASTELAND_CONTAMINATION_AT_BEGINNING
 */
double generate_random_wasteland_contamination_level();

/**
 * @brief Test if all indexes of an array are taken.
 * @param indexes_taken An array that represents the indexes taken or not of another array.
 * @return 1 if all indexes are taken / 0 if not.
 */
int all_tile_indexes_are_taken(int indexes_taken[CITY_WIDTH][CITY_HEIGHT]);

/**
 * @brief Contaminate some wastlands in the city.
 * @param city City to contaminate.
 */
void contaminate_some_wastelands(city_t *city);

#endif
