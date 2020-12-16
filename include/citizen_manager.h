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
 * @version 1.0.0 - 2020-12-16
 */

/**
 * @file citizen_manager.h
 *
 * Manage the citizens of the city.
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

#define PROB_TO_STAY_ON_TILE 0.6

#define CONTAMINATION_INCREASE_MOVE_CITIZEN 0.02
#define CONTAMINATION_INCREASE_STAY_CITIZEN 0.05
#define CONTAMINATION_INCREASE_TILE 0.1

#define HOSPITAL_CONTAMINATION_REDUCTION 0.25

enum { STAY, MOVE };

/**
 * @brief Handler function for a doctor thread.
 * @param status Status of the doctor.
 */
void *doctor_process(void *status);

/**
 * @brief Handler function for a fireman thread.
 * @param status Status of the fireman.
 */
void *fireman_process(void *status);

/**
 * @brief Handler function for a journalist thread.
 * @param status Status of the journalist.
 */
void *journalist_process(void *status);

/**
 * @brief Handler function for a simple citizen thread.
 * @param status Status of the simple citizen.
 */
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
 * @brief Initialize a citizen.
 * @param status Status of the citizen.
 * @param type The type of the citizen.
 */
void init_citizen(status_t *status, citizen_type_e type);

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
 * @brief Remove a citizen from a tile.
 * @param tile Tile where to remove a citizen.
 */
void remove_citizen_from_tile(tile_t *tile);

/**
 * @brief Update the coordinates of a citizen.
 * @param status Status of the citizen.
 * @param x The new x coordinate in the city.
 * @param y The new y coordinate in the city.
 */
void update_citizen_coords(status_t *status, unsigned int x, unsigned int y);

/**
 * @brief Increment a global variable that counts the number of doctors initialized in a hospital.
 */
void increment_init_doctors_in_hospital();

/**
 * @brief Increment a global variable that counts the number of firemen initialized in a firestation.
 */
void increment_init_firemen_in_firestation();

/**
 * @brief Count the number of firemen in a firestation.
 * @param col The x coordinate in the city.
 * @param row The y coordinate in the city.
 * @return -1 if the tile is not a FIRESTATION / else the number of firemen.
 */
int firemen_nb_in_firestation(unsigned int col, unsigned int row);

/**
 * @brief Count the number of visitors in a firestation.
 * @param col The x coordinate in the city.
 * @param row The y coordinate in the city.
 * @return -1 if the tile is not a FIRESTATION / else the number of visitors.
 */
int visitors_nb_in_firestation(unsigned int col, unsigned int row);

/**
 * @brief Test if a citizen is allowed to enter in a hospital.
 * @return 1 if the citizen is allowed / 0 if not.
 */
int is_allowed_to_enter_in_a_hospital(status_t *status);

/**
 * @brief Citizen move from or stay on his tile dependending on chance and on his status.
 * @param status Status of the citizen.
 */
void move_citizen(status_t *status);

/**
 * @brief Test if a citizen can leave a tile.
 * @param status Status of the citizen.
 * @return 1 if the citizen can leave / 0 if not.
 */
int citizen_can_leave_tile(status_t *status);

/**
 * @brief Test if a citizen can enter a tile.
 * @param status Status of the citizen.
 * @param tile Tile where the citizen want to enter.
 * @return 1 if the citizen can enter / 0 if not.
 */
int citizen_can_enter_tile(status_t *status, tile_t *tile);

/**
 * Generate a possible new position for a citizen.
 * @param status Status of the citizen.
 * @param new_x Possible new x coordinate in the city.
 * @param new_y Possible new y coordinate in the city.
 */
void generate_new_citizen_position(status_t *status, int* new_x, int* new_y);

/**
 * @brief Increase the contamination of a tile with the contamination level of a citizen.
 * @param status Status of the citizen.
 */
void increase_tile_contamination(status_t *status);

/**
 * @brief Increase the contamination of a citizen.
 * @param status Status of the citizen.
 * @param move Indicate if the citizen has moved or not.
 */
void increase_citizen_contamination(status_t *status, int move);

/**
 * @brief Initialize all the threads of the citizens.
 */
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
int tile_is_full(tile_t *tile);

/**
 * @brief Unblocks citizens at each new round.
 */
void citizens_simulation();

/**
 * @brief Wait for all the citizens to end.
 */
void wait_for_citizens_to_end();

#endif
