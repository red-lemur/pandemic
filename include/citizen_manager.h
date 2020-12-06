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
 * @version 1.0.0 - 2020-12-07
 */

/**
 * @file citizen_manager.h
 *
 *
 */

#ifndef CITIZEN_MANAGER_H
#define CITIZEN_MANAGER_H

#include "city.h"

#define DOCTORS_NB 4
#define FIREMEN_NB 6
#define JOURNALISTS_NB 2
#define SIMPLE_CITIZENS_NB CITIZENS_NB - DOCTORS_NB - FIREMEN_NB - JOURNALISTS_NB

/**/
void *doctor_process(void *status);
void *fireman_process(void *status);
void *journalist_process(void *status);
void *simple_citizen_process(void *status);

/**/
void init_population(city_t *city);

#endif
