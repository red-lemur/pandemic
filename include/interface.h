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
 * @file interface.h
 *
 *
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "city.h"

#define MARGIN 10
#define VERTICAL_MARGIN 2
#define BOTTOM_MARGIN 5

#define HEADER_HEIGHT 3
#define TITLES_HEIGHT 3

#define SPACE_SITUATION 3

#define NUMBER_OF_DIF_TILES 4
#define NUMBER_OF_SITUATIONS 4

#define SIZE_OF_TITLE 32

#define NUMBER_OF_DAYS 10

enum { DEFAULT, WASTELAND_CODE, HOUSE_CODE, HOSPITAL_CODE, FIRESTATION_CODE, HEALTHY_CODE,
       SICK_CODE, DEAD_CODE, BURNED_CODE, TITLE };
enum { UP_LEFT, UP, UP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM, BOTTOM_LEFT, LEFT };
enum { HEALTHY_STATE, SICK_STATE, DEAD_STATE, BURNED_STATE };

/* COMMENTAIRES */
int get_tile_color_code(int tile);

int get_state_color_code(int state);

void write_space(WINDOW *win, int y, int x, int color_index);

int size_of_longest_string(char **strings, int size);

void next_day();

void set_number_of_people_in_state(int number, int state);

void set_citizen_on_tile(int tile_x, int tile_y, int number, int state);

void set_type_of_tile(int tile_x, int tile_y, int type);

#endif
