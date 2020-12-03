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
 * @version 1.0.0 - 2020-12-03
 */

/**
 * @file epidemic_sim.h
 *
 * 
 */

#ifndef EPIDEMIC_SIM_H
#define EPIDEMIC_SIM_H

#include "city_sim.h"

#define SHARED_MEM "/simulation"

/**
 * @brief Create the shared memory.
 */
int create_shared_memory();

/**
 * @brief Generate the city in which the simulation takes place.
 * @param city City to generate.
 */
void generate_city(city_t *city);

/**
 * @brief Load the map of the city from a text file.
 * @param city City to load.
 */
void load_map(city_t *city);

/**
 * @brief Generate a random contamination level to initiate a tile of WASTELAND type.
 * return A contamination level between MIN and MAX_WASTELAND_CONTAMINATION_AT_BEGINNING
 */
double generate_random_wasteland_contamination_level();

/**
 * @brief Generate a random index for an array.
 * @param ind_max Maximal index to generate.
 * @return An index between 0 and ind_max excluded.
 */
int generate_random_index(int ind_max);

/**
 * @brief Test if all indexes of an array are taken.
 * @param indexes_taken An array that represents the indexes taken or not of another array.
 * @return 1 if all indexes are taken / 0 if not.
 */
int all_tile_indexes_are_taken(int indexes_taken[CITY_WIDTH][CITY_HEIGHT]);
    
#endif
