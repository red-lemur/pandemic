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
 * @file city.h
 *
 * Constants and functions to manipulate a simulated city.
 */

#ifndef CITY_SIM_H
#define CITY_SIM_H

#include "city.h"

#define WASTELAND_CAPACITY 16
#define HOUSE_CAPACITY 6
#define HOSPITAL_CAPACITY 12
#define FIRESTATION_CAPACITY 8

#define CITIZEN_NB_AT_BEGINNING 0
#define CONTAMINATION_AT_BEGINNING 0
#define MIN_WASTELAND_CONTAMINATION_AT_BEGINNING 0.2
#define MAX_WASTELAND_CONTAMINATION_AT_BEGINNING 0.4

#define HOSPITALS_NB 1
#define FIRESTATIONS_NB 2
#define HOUSES_NB 12

#define MAP_URL "./etc/map.txt"

/**
 * @brief Initialize a tile.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @param capacity The max number of citizens on this tile.
 * @param type The type of the building on the tile.
 * @param contamination The contamination level on the tile.
 * @return the intialized tile.
 */
tile_t init_tile(unsigned int x, unsigned int y, int capacity,
                 building_type_e type, double contamination);

/**
 * @brief Initialize a tile of WASTELAND type.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @param contamination The contamination level on the tile.
 * @return the intialized tile.
 */
tile_t init_tile_wasteland(unsigned int x, unsigned int y, double contamination);

/**
 * @brief Initialize a tile of HOUSE type.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @return the intialized tile.
 */
tile_t init_tile_house(unsigned int x, unsigned int y);

/**
 * @brief Initialize a tile of HOSPITAL type.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @return the intialized tile.
 */
tile_t init_tile_hospital(unsigned int x, unsigned int y);

/**
 * @brief Initialize a tile of FIRESTATION type.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @return the intialized tile.
 */
tile_t init_tile_firestation(unsigned int x, unsigned int y);

#endif
