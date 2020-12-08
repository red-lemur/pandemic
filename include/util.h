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
 * @version 1.0.0 - 2020-12-08
 */

/**
 * @file utils.h
 *
 * Reusable functions.
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * @brief Generate a random index for an array.
 * @param ind_max Maximal index to generate.
 * @return An index between 0 and ind_max excluded.
 */
int generate_random_index(int ind_max);

/**
 * @brief Generate a random percentage in an interval.
 * @param lower_bound Lower bound of the interval.
 * @param upper_bound Upper bound of the interval.
 * @return A double between the lower bound and the upper bound.
 */
double generate_random_percentage_in_interval(double lower_bound, double upper_bound);

/**
 * @brief Generate a random percentage.
 * @return A double between 0 and 1.
 */
double generate_random_percentage();

#endif
