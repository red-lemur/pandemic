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

#include <ncurses.h>

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

/**
 * @brief Converts a tile code into its color code
 * @param tile code of the tile.
 */
int get_tile_color_code(int tile);

/**
 * @brief Converts a state code into its color code
 * @param state code of the state
 */
int get_state_color_code(int state);


/**
 * @brief write an empty space to symbolize a tile on given coordinates
 * @param win windows in which the space will be added
 * @param y y coordinate where the space will be added
 * @param x x coordinate where the space will be added
 * @param color_index color code of the space
 */
void write_space(WINDOW *win, int y, int x, int color_index);

/**
 * @brief Returns the size of the longest string among an array of strings
 * @param strings array containing all of the strings
 * @param size size of the array
 */
int size_of_longest_string(char **strings, int size);

/**
 * @brief Replace the display of the current day by the next one at the top of the window
 */
void next_day();

/**
 * @brief Changes the total number of people in a specified state
 * @param number the new number of people in the specified state
 * @param state code of the state
 */
void set_number_of_people_in_state(int number, int state);

/**
 * @brief Changes the total number of people on a specified tile
 * @param tile_x x coordinate of the tile
 * @param tile_y y coordinate of the tile
 * @param number the new number of people on the specified tile
 * @param state code of the state
 */
void set_citizen_on_tile(int tile_x, int tile_y, int number, int state);

/**
 * @brief Calls every functions that will initialize each part of the interface
 */
void create_interface();

/**
 * @brief Initializes the interface, every color code and clears the screen
 */
void initialize_interface();

/**
 * @brief Initializes a window at the top and writes the title into it
 */
void initialize_main_title();

/**
 * @brief Initializes two windows and writes four titles that will correspond to the four main windows
 */
void initialize_titles();

/**
 * @brief Initializes a window at the top left of the window and displays the map into it
 */
void initialize_places();

/**
 * @brief Initializes a window at the right of the map and writes a legend for the map
 */
void initialize_legend();

/**
 * @brief Initializes a window at the bottom left of the window and displays the location of every citizen into it
 */
void initialize_citizens();

/**
 * @brief Initializes a window at the right of the citizen map and writes how many citizen are in what state
 */
void initialize_situations();

/**
 * @brief Updates the place of citizens and their states with the city in the shared memory
 */
void update_interface();

/**
 * @brief Deletes all windows and quits the interface
 */
void end_interface();

#endif
