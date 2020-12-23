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
 * @brief Close everything that has been opened and end the program.
 */
void end_press_agency();

/**
 * @brief Print the header of the press agency on the terminal.
 */
void print_header();

#endif
