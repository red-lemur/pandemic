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
 * @file epidemic_sim.h
 *
 * The most important process of the simulation. Manage the city.
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

/**
 * @brief Update the contamination level of the WASTELAND tiles.
 */
void update_wastelands_contamination();

/**
 * @brief Increase the contamination of a WASTELAND tile based on another tile contamination.
 * @param tile Tile whose contamination must be increased.
 * @param other_tile_contamination Contamination of another tile.
 */
void increase_wasteland_contamination(tile_t *tile, double other_tile_contamination);

/**
 * @brief Hospitals can heal some citizens each round.
 */
void hospitals_heal();

/**
 * @brief Heal some citizens on an hospital tile.
 * @param tile Tile where the hospital is.
 */
void hospital_heal_tile(tile_t *tile);

/**
 * @brief Return the number of healthy doctors in a tile.
 * @param tile Tile where to search the doctors.
 * @return The number of healthy doctors.
 */
int get_healthy_doctors_nb_on_tile(tile_t *tile);

/**
 * @brief Return the sickest citizen on a given tile, if he exist.
 * @param tile Tile where to search the sickest citizen.
 * @return The sickest citizen if he exist / else null.
 */
status_t *get_sickest_citizen_of_tile(tile_t *tile);

/**
 * @brief Heal a citizen.
 * @param status Status of the citizen.
 */
void heal_citizen(status_t *status);

/**
 * @brief Create and launch the interface.
 */
void launch_interface();

/**
 * @brief Infinite loop while the simulation is not over.
 */
void launch_simulation();

/**
 * @brief Actions taken during a simulation round.
 */
void simulation_round();

/**
 * @brief End the simulation.
 */
void end_of_simulation();

#endif
