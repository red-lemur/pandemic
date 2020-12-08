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
 * @file epidemic_sim.h
 *
 * 
 */

#ifndef EPIDEMIC_SIM_H
#define EPIDEMIC_SIM_H

#include "city.h"
#include "map_generator.h"

#include <signal.h>

#define PROBABILITY_WASTELAND_CONTAMINATION 0.15
#define MIN_WASTELAND_CONTAMINATION_INCREASE 0.01
#define MAX_WASTELAND_CONTAMINATION_INCREASE 0.20

/**
 * @brief Create the shared memory.
 */
int create_shared_memory();

/**/
void update_wastelands_contamination();

/**
 * @brief Increase the contamination of a WASTELAND tile based on another tile contamination.
 * @param tile Tile whose contamination must be increased.
 * @param other_tile_contamination Contamination of another tile.
 */
void increase_wasteland_contamination(tile_t *tile, double other_tile_contamination);

/**/
void launch_simulation();

/**/
void simulation_round();

/**/
void end_of_simulation();

#endif
