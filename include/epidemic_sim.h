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

#include "city.h"
#include "map_generator.h"

#include <signal.h>

#define SHARED_MEM "/simulation"

struct sigaction action_sigusr1;
struct sigaction action_sigusr2;

int game_is_not_over = 1;

/**
 * @brief Create the shared memory.
 */
int create_shared_memory();

/**
 * @brief Generate the city in which the simulation takes place.
 * @param city City to generate.
 */
void generate_city(city_t *city);

/**/
void game_round();

/**/
void end_of_game();

#endif
