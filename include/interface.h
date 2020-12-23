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
 * @version 1.0.0 - 2020-12-23
 */

/**
 * @file interface.h
 *
 * Interface between the user and the simulation.
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

#define SIZE_OF_TITLE 29

#define NUMBER_OF_DAYS 10

enum { DEFAULT, WASTELAND_CODE, HOUSE_CODE, HOSPITAL_CODE, FIRESTATION_CODE, HEALTHY_CODE,
       SICK_CODE, DEAD_CODE, BURNED_CODE, TITLE };
enum { UP_LEFT, UP, UP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM, BOTTOM_LEFT, LEFT };
enum { HEALTHY_STATE, SICK_STATE, DEAD_STATE, BURNED_STATE };

/**
 * @brief Convert a tile code into its color code.
 * @param tile Code of the tile.
 */
int get_tile_color_code(int tile);

/**
 * @brief Convert a state code into its color code.
 * @param state Code of the state.
 */
int get_state_color_code(int state);

/**
 * @brief Write an empty space to symbolize a tile on given coordinates.
 * @param win Windows in which the space will be added.
 * @param y Y coordinate where the space will be added.
 * @param x X coordinate where the space will be added.
 * @param color_index Color code of the space.
 */
void write_space(WINDOW *win, int y, int x, int color_index);

/**
 * @brief Return the size of the longest string among an array of strings.
 * @param strings Array containing all of the strings.
 * @param size Size of the array.
 */
int size_of_longest_string(char **strings, int size);

/**
 * @brief Replace the display of the current day by the next one at the top of the window.
 * @param round_nb Num of the round.
 */
void next_day(int round_nb);

/**
 * @brief Change the total number of people in a specified state.
 * @param number The new number of people in the specified state.
 * @param state Code of the state.
 */
void set_number_of_people_in_state(int number, int state);

/**
 * @brief Change the total number of people on a specified tile.
 * @param tile_x X coordinate of the tile.
 * @param tile_y Y coordinate of the tile.
 * @param number The new number of people on the specified tile.
 * @param state Code of the state.
 */
void set_citizen_on_tile(int tile_x, int tile_y, int number, int state);

/**
 * @brief Call every functions that will initialize each part of the interface.
 * @param city City to display.
 */
void create_interface(city_t *city);

/**
 * @brief Initialize the interface, every color code and clears the screen.
 */
void initialize_interface();

/**
 * @brief Initialize a window at the top and writes the title into it.
 */
void initialize_main_title();

/**
 * @brief Initialize two windows and writes four titles that will correspond to the four main windows.
 */
void initialize_titles();

/**
 * @brief Initialize a window at the top left of the window and displays the map into it.
 * @param city City to display.
 */
void initialize_places(city_t *city);

/**
 * @brief Initialize a window at the right of the map and writes a legend for the map.
 */
void initialize_legend();

/**
 * @brief Initialize a window at the bottom left of the window and displays the location of every citizen into it.
 */
void initialize_citizens();

/**
 * @brief Initialize a window at the right of the citizen map and writes how many citizen are in what state.
 */
void initialize_situations();

/**
 * @brief Change the population states with the array given.
 * @param state_counters Array with the number of people in each state.
 */
void update_population_states(int state_counters[NUMBER_OF_SITUATIONS]);

/**
 * @brief Change the population map with the array given.
 * @param population Array with the number of people on each tile and different state.
 */
void update_population_map(int population[CITY_WIDTH][CITY_HEIGHT][NUMBER_OF_SITUATIONS]);

/**
 * @brief Fill the two arrays with the city in shared memory.
 * @param population Array that will contain the number of people on each tile and different state.
 * @param state_counters Array that will contain the number of people in each state.
 * @param city City that contains the people.
 */
void fill_arrays_with_city(int population[CITY_WIDTH][CITY_HEIGHT][NUMBER_OF_SITUATIONS],
                           int state_counters[NUMBER_OF_SITUATIONS], city_t *city);

/**
 * @brief Update the place of citizens and their states with the city in the shared memory.
 * @param round_nb Num of the round.
 * @param city City to display.
 */
void update_interface(int round_nb, city_t *city);

/**
 * @brief Delete all windows and quit the interface.
 */
void end_interface();

/**
 * @brief Clear the terminal in case something went wrong.
 */
void clear_terminal();

#endif
