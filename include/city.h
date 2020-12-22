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
 * @author Alain Lebret <alain.lebret@ensicaen.fr> [original author]
 * @author Jérémy Poullain <jeremy.poullain@ecole.ensicaen.fr>
 * @author Guillaume Revel <guillaume.revel@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-12-19
 */

/**
 * @file city.h
 *
 * Structures defining the city.
 */

#ifndef CITY_H
#define CITY_H

#define CITY_WIDTH 7
#define CITY_HEIGHT 7

#define CITIZENS_NB 37
#define CITIZEN_NAME_MAX_LENGTH 42

typedef enum { WASTELAND, HOUSE, HOSPITAL, FIRESTATION } building_type_e;
typedef enum { SIMPLE_CITIZEN, FIREMAN, DOCTOR, JOURNALIST, DEAD, BURNED } citizen_type_e;

/**
 * @brief Defines the status of a citizen.
 * @param id ID of the citizen for him to be unique.
 * @param name The name of the citizen.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @param contamination The level of contamination (between 0 and 1).
 * @param is_sick A boolean to say if the citizen is sick.
 * @param sickness_duration The number of days the citizen have been sick.
 * @param type The type of the citizen.
 * @param days_spend_in_hospital_healthy the number of days spent in the hospital while not being sick.
 * @param must_leave Indicates if the citizen must leave his tile.
 * @param doctor_can_enter_hospital A doctor who leave the hospital can come back at least two days after.
 * @param treatment_pouches_nb Number of treatment pouches if the citizen is a DOCTOR.
 * @param sprayer_capacity Sprayer capacity if the citizen is a FIREMAN.
 */
typedef struct status {
    unsigned int id;
    char name[CITIZEN_NAME_MAX_LENGTH];
    unsigned int x;
    unsigned int y;
    double contamination;
    int is_sick;
    int sickness_duration;
    citizen_type_e type;
    int days_spent_in_hospital_healthy;
    int must_leave;
    int doctor_can_enter_hospital;
    int treatment_pouches_nb;
    double sprayer_capacity;
} status_t;

/**
 * @brief Represents a tile on the city.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @param capacity The max number of citizens on this tile.
 * @param citizens_nb The current number of citizens on the tile.
 * @param type The type of the building on the tile.
 * @param contamination The contamination level on the tile.
 */
typedef struct tile {
    unsigned int x;
    unsigned int y;
    int capacity;
    int citizens_nb;
    building_type_e type;
    double contamination;
} tile_t;

/**
 * @brief Represents the city in which the epidemic occurs.
 * @param map A 2D array of tiles representing the city.
 * @param citizens An array of status representing the citizens.
 * @param wasteland_tiles_nb Number of WASTELAND tiles.
 * @param house_tiles_nb Number of HOUSE tiles.
 * @param hospital_tiles_nb Number of HOSPITAL tiles.
 * @param firestation_tiles_nb Number of FIRESTATION tiles.
 * @param citizens_sick_number Number of citizens currently sick in the city.
 * @param city_mean_contamination Mean contamination of the city.
 * @param deads_number Actual deads number in the city.
 */
typedef struct city {
    tile_t map[CITY_WIDTH][CITY_HEIGHT];
    status_t citizens[CITIZENS_NB];
    int wasteland_tiles_nb;
    int house_tiles_nb;
    int hospital_tiles_nb;
    int firestation_tiles_nb;
    int citizens_sick_number;
    double city_mean_contamination;
    int deads_number;
} city_t;

#endif
