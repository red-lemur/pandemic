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
 * @version 1.0.0 - 2020-12-28
 */

/**
 * @file utils.c
 *
 * Reusable functions.
 */

#include <stdlib.h>

#include "util.h"

int generate_random_int_in_interval(int min, int max)
{
        return (int)(rand()/(double)RAND_MAX * (max + 1 - min) + min);
}

int generate_random_index(int ind_max)
{
        return (int)(rand()/(double)RAND_MAX * ind_max);
}

double generate_random_percentage_in_interval(double lower_bound, double upper_bound)
{
        return rand()/(double)RAND_MAX * (upper_bound - lower_bound) + lower_bound;
}

double generate_random_percentage()
{
        return generate_random_percentage_in_interval(0, 1);
}
