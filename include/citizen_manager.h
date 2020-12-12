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

/**
 * @brief Initialize a doctor.
 * @param status Status of the doctor.
 * @param treatment_pouches_nb Number of treatment pouches of the doctor.
 **/
void init_doctor(status_t *status, int* treatment_pouches_nb);

/**
 * @brief Initialize a fireman.
 * @param status Status of the fireman.
 * @param sprayer_capacity Capacity of the sprayer of the fireman.
 **/
void init_fireman(status_t *status, double *sprayer_capacity);

/**
 * @brief Initialize the status of a citizen.
 * @param status Status of the citizen which contains several paramters.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @param type The type of the citizen.
 */
void init_citizen_status(status_t *status, int x, int y, citizen_type_e type);

/**
 * @brief Add a citizen in a tile.
 * @param tile Tile where to add the citizen.
 */
void add_citizen_in_tile(tile_t *tile);

/**
 * @brief Increment a global variable that counts the number of doctors initialized in a hospital.
 */
void increment_init_doctors_in_hospital();

/**
 * @brief Increment a global variable that counts the number of firemen initialized in a firestation.
 */
void increment_init_firemen_in_firestation();

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
