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

/* COMMENTAIRE */
void load_map(city_t *city);

/**/
double generate_random_wasteland_contamination_level();

#endif
