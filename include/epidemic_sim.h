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
 * @version 1.0.0 - 2020-12-07
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

/**
 * @brief Create the shared memory.
 */
int create_shared_memory();

/**/
void launch_simulation();

/**/
void simulation_round();

/**/
void end_of_simulation();

#endif
