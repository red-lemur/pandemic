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
 * @version 1.0.0 - 2020-12-05
 */

/**
 * @file interface.h
 *
 *
 */

#define MARGIN 10
#define VERTICAL_MARGIN 2
#define HEADER_HEIGHT 3
#define TITLES_HEIGHT 3
#define MAP_HEIGHT 7
#define SPACE_SITUATION 3
#define BOTTOM_MARGIN 5
#define POPULATION_NUMBER 25
#define NUMBER_OF_DIF_TILES 4
#define NUMBER_OF_SITUATIONS 4
#define SIZE_OF_TITLE 32

#define NUMBER_OF_DAYS 10

#define HOUSE 2
#define HOSPITAL 3
#define FIRESTATION 4
#define WASTELAND 5

#define HEALTHY 6
#define SICK 7
#define DEAD 8
#define BURNT 9

#define UP_LEFT 1
#define UP 2
#define UP_RIGHT 3
#define RIGHT 4
#define BOTTOM_RIGHT 5
#define BOTTOM 6
#define BOTTOM_LEFT 7
#define LEFT 8

void write_spaces(WINDOW *win, int y, int x, int number, int color_index);

int size_of_longest_string(char **strings, int size);

void next_day();

void set_number_of_people_in_state(int number, int state);

void set_citizen_on_tile(int tile_x, int tile_y, int number, int state);

void set_type_of_tile(int tile_x, int tile_y, int type);
