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
 * @file timer.h
 *
 * 
 */

#ifndef TIMER_H
#define TIMER_H

#define TOTAL_TOUR_NB 100
#define TOUR_DURATION 5

struct sigaction action;

int tour_nb = 0;

/**
 * @brief Emit a signal which call this function again.
 */
void tick();

#endif
