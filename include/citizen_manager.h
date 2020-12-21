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
 * @version 1.0.0 - 2020-12-21
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
#define MAX_SPRAYER_DECONTAMINATION 0.2

#define TREATMENT_POUCHES_NB_AT_BEGINNING 5
#define SPRAYER_CAPACITY_AT_BEGINNING 5

#define PROB_TO_STAY_ON_TILE 0.6

#define DAYS_NB_DOCTOR_HAS_TO_WAIT 2
#define DAYS_NB_SICK_DOCTOR_CAN_HEAL_HIMSELF 10

#define CONTAMINATION_INCREASE_MOVE_CITIZEN 0.02
#define CONTAMINATION_INCREASE_STAY_CITIZEN 0.05
#define CONTAMINATION_INCREASE_TILE 0.1
#define CONTAMINATION_CITIZEN_BY_CITIZEN 0.01

#define CONTAMINATION_DECREASE_IN_FIRESTATION 0.2
#define CONTAMINATION_DECREASE_IN_HOSPITAL 0.1

#define PROB_CONTAMINATE_IF_SICK_TILE 0.1
#define PROB_CONTAMINATE_IF_SICK_WASTELANDS_AROUND 0.01

#define HOSPITAL_CONTAMINATION_REDUCTION 0.25
#define MAX_DAYS_IN_HOSPITAL_HEALTHY 2

#define FIREMAN_PROTECTION_FACTOR 0.1
#define FIREMAN_PROTECTION_CITIZEN 0.7

#define DAYS_NB_DISEASE_DEADLY 5
#define PROB_OF_DEATH_BY_DAY 0.05

#define DEATH_REDUCTION_IF_DOCTOR 0.5
#define DEATH_REDUCTION_IF_HOSPITAL 0.25

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
 * @brief Every citizen does the same actions during a round.
 * @param status Status of the citizen.
 */
void every_citizen_round(status_t *status);

/**
 * @brief Initialize a doctor.
 * @param status Status of the doctor.
 */
void init_doctor(status_t *status);

/**
 * @brief Initialize a fireman.
 * @param status Status of the fireman.
 */
void init_fireman(status_t *status);

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
 * @brief Increment a global variable that counts the number of doctors initialized in a hospital.
 */
void increment_init_doctors_in_hospital();

/**
 * @brief Increment a global variable that counts the number of firemen initialized in a firestation.
 */
void increment_init_firemen_in_firestation();

/**
 * @brief Refill the treatment pouches of a doctor.
 * @param status Status of the citizen.
 */
void refill_treatment_pouches(status_t *status);

/**
 * @brief Refill the sprayer of a fireman.
 * @param status Status of the citizen.
 */
void refill_sprayer(status_t *status);

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
 * @brief Count the number of doctors in a tile.
 * @param tile Tile where to count the doctors.
 * @return The number of doctors.
 */
int doctor_nb_in_tile(tile_t* tile);

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
 * @brief Healed citizen must leave the hospital if it is full.
 * @param tile Tile the hospital is.
 */
void healed_citizens_must_leave_hospital(tile_t* tile);

/**
 * Generate a possible new position for a citizen.
 * @param status Status of the citizen.
 * @param new_x Possible new x coordinate in the city.
 * @param new_y Possible new y coordinate in the city.
 */
void generate_new_citizen_position(status_t *status, unsigned int* new_x, unsigned int* new_y);

/**
 * @brief Depending on the tile he is on, the contamination of a citizen can decrease.
 * @param status Status of the citizen.
 */
void tile_decrease_citizen_contamination(status_t *status);

/**
 * @brief A fireman decreases a tile with its sprayer.
 * @param status Status of the citizen.
 */
void sprayer_decontamination(status_t *status);

/**
 * @brief Return the most contaminated citizen on a given tile, if he exist.
 * @param fireman_status Fireman currently decontaminating.
 * @param tile Tile where to search the most contaminated citizen.
 * @return The most contaminated citizen if he exist / else null.
 */
status_t *get_most_contaminated_citizen_of_tile(status_t *fireman_status, tile_t *tile);

/**
 * @brief Decontaminate a tile with a sprayer.
 * @param status Status of the fireman.
 * @param tile Tile to be decontaminated.
 * @param decontamination Decontamination to be applied.
 */
void decontamine_tile_with_sprayer(status_t *status, tile_t *tile, double decontamination);

/**
 * @brief Decontaminate a citizen with a sprayer.
 * @param fireman_status Status of the fireman.
 * @param citizen_status Status of the citizens to be decontaminated.
 * @param decontamination Decontamination to be applied.
 */
void decontamine_citizen_with_sprayer(status_t *fireman_status, status_t *citizen_status,
                                      double decontamination);
/**
 * @brief A ctitizen who is a doctor can heal other citizens under some conditions.
 * @param status Status of the citizen.
 */
void doctor_can_heal(status_t *status);

/**
 * @brief A doctor heals a citizen.
 * @param doctor_status Status of the doctor.
 * @param citizen_status Status of the citizen.
 */
void heal(status_t *doctor_status, status_t *citizen_status);

/**
 * @brief Return the sickest citizen on a given tile, if he exist.
 * @param tile Tile where to search the sickest citizen.
 * @return The sickest citizen if he exist / else null.
 */
status_t *get_sickest_citizen_of_tile(tile_t *tile);

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
 * @brief A citizen has a chance to contaminate other citizens if he is sick.
 * @param status Status of the citizen.
 */
void contaminate_other_citizens_if_sick(status_t *status);

/**
 * @brief A citizen contaminate the citizens on his tile.
 * @param status Status of the citizen.
 */
void contaminate_citizens_in_same_tile(status_t *status);

/**
 * @brief A citizen contaminate the citizens in WASTELANDS around.
 * @param status Status of the citizen.
 */
void contaminate_citizens_in_wastelands_around(status_t *status);

/**
 * @brief If a citizen is a FIREMAN, he can burn the dead in a tile he enters.
 * @param status Status of the citizen.
 */
void burn_the_dead(status_t *status);

/**
 * @brief A citizen gets sick if he is too contaminated.
 * @param status Status of the citizen.
 */
void risk_of_disease(status_t *status);

/**
 * @brief Increment the number of days a citizen has been sick.
 * @param status Status of the citizen.
 */
void increment_sickness_duration(status_t *status);

/**
 * @brief A citizen dies if he is too sick.
 * @param status Status of the citizen.
 */
void risk_of_death(status_t *status);

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
