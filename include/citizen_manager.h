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
 * @file citizen_manager.h
 *
 *
 */

#ifndef CITIZEN_MANAGER_H
#define CITIZEN_MANAGER_H

#include "city.h"

#define DOCTORS_NB 4
#define FIREMEN_NB 6
#define JOURNALISTS_NB 2
#define SIMPLE_CITIZENS_NB CITIZENS_NB - DOCTORS_NB - FIREMEN_NB - JOURNALISTS_NB

#define MAX_TREATMENT_POUCHES_NB 10
#define MAX_SPRAYER_CAPACITY 10
#define MAX_SPRAYER_USE 0.1

#define TREATMENT_POUCHES_NB_AT_BEGINNING 5
#define SPRAYER_CAPACITY_AT_BEGINNING 5

/**/
void *doctor_process(void *status);
void *fireman_process(void *status);
void *journalist_process(void *status);
void *simple_citizen_process(void *status);

/**/
void init_doctor(status_t *status, int* treatment_pouches_nb);

/**/
void init_citizen(status_t *status, int x, int y, citizen_type_e type);

/**/
void add_citizen_in_tile(tile_t *tile);

/**
 * @brief Increment a global variable that counts the nuber of doctors initialized in a hospital.
 */
void increment_init_doctor_in_hospital();

/**/
void move_citizen();

/**/
void init_population();

/**
 * @brief Increment a counter when a citizen ends.
 */
void citizen_ended();

/**
 * @brief Check if a tile is full or not.
 * @param tile Tile to check.
 * @return 1 if the tile is full of citizen / 0 if not.
 */
int tile_is_full(tile_t tile);

#endif
