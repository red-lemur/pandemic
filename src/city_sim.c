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
 * @version 1.0.0 - 2020-12-28
 */

/**
 * @file city_sim.c
 *
 * Functions to manipulate a simulated city.
 */

#include "city_sim.h"

tile_t init_tile(unsigned int x, unsigned int y, int capacity,
                 building_type_e type)
{
        tile_t tile;
        
        tile.x = x;
        tile.y = y;
        tile.capacity = capacity;
        tile.citizens_nb = CITIZEN_NB_AT_BEGINNING;
        tile.type = type;
        tile.contamination = CONTAMINATION_AT_BEGINNING;
        
        return tile;
}

tile_t init_tile_wasteland(unsigned int x, unsigned int y)
{
        return init_tile(x, y, WASTELAND_CAPACITY, WASTELAND);
}

tile_t init_tile_house(unsigned int x, unsigned int y)
{
        return init_tile(x, y, HOUSE_CAPACITY, HOUSE);
}

tile_t init_tile_hospital(unsigned int x, unsigned int y)
{
        return init_tile(x, y, HOSPITAL_CAPACITY, HOSPITAL);
}

tile_t init_tile_firestation(unsigned int x, unsigned int y)
{
        return init_tile(x, y, FIRESTATION_CAPACITY, FIRESTATION);
}
