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
 * @file press_agency.h
 * 
 * Receive news from the journalists who circulate in the city and post them continuously.
 */

#ifndef PRESS_AGENCY_H
#define PRESS_AGENCY_H

#include <mqueue.h>

#define MIN_JOURNALIST_CONTAMINATION_TO_DISPLAY 0.8

#define CONTAMINATION_MINORATION 0.1
#define DEADS_MINORATION 0.35

/**
 * @brief Create the memory queue.
 * @return The memory queue for exchanges between journalists and the press agency.
 */
mqd_t create_mqueue();

/**
 * @brief Allocate the parameters for the mq_receive() function.
 */
void allocate_receive_parameters();

/**
 * @brief The press agency receives in a loop news from the journalists.
 */
void receive_news();

/**
 * @brief Store the news to print them later.
 */
void store_news();

/**
 * @brief Check if all the journalists are dead.
 * @return 1 if all the journalists are dead / 0 if not.
 */
int all_journalists_are_dead();

/**
 * @brief Print the header of the press agency on the terminal.
 */
void print_header();

/**
 * @brief Print the news on the terminal.
 */
void print_news();

/**
 * @brief This happen when all the journalists are dead.
 */
void press_agency_zombie();

/**
 * @brief Close everything that has been opened and end the program.
 */
void end_press_agency();

#endif
