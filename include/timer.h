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
 * @version 1.0.0 - 2020-12-19
 */

/**
 * @file timer.h
 *
 * A clock that emit an alarm signal every tour of a given duration.
 */

#ifndef TIMER_H
#define TIMER_H

#include <signal.h>

#define TOTAL_TOUR_NB 50 // 100

/**
 * @brief Emit a signal which call this function again.
 */
void tick();

#endif
