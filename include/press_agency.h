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
 * @version 1.0.0 - 2020-12-22
 */

/**
 * @file press_agency.h
 * 
 * 
 */

#ifndef PRESS_AGENCY_H
#define PRESS_AGENCY_H

#include <mqueue.h>

/**
 * @brief Create the memory queue.
 * @return The memory queue for exchanges between journalists and the press agency.
 */
mqd_t create_mqueue();

#endif
