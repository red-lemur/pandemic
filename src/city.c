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
 * @version 1.0.0 - 2020-11-30
 */

/**
 * @file city.c
 *
 * Structures defining the city.
 */

#include "city.h"

tile_t init_tile(unsigned int x, unsigned int y, int capacity, int citizens_nb,
                 building_type_e type, double contamination) {
    tile_t tile;
    
    tile.x = x;
    tile.y = y;
    tile.capacity = capacity;
    tile.citizens_nb = citizens_nb;
    tile.type = type;
    tile.contamination = contamination;

    // DES TRUCS A CHANGER
    ...
    
    return tile;
}
