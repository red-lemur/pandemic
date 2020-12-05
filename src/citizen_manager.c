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
 * @author Alain Lebret <alain.lebret@ensicaen.fr> [original author]
 * @author Jérémy Poullain <jeremy.poullain@ecole.ensicaen.fr>
 * @author Guillaume Revel <guillaume.revel@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-11-30
 */

/**
 * @file citizen_manager.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "epidemic_sim.h"
#include "city.h"

/*void create_population(city_t *city) { }*/

int main(void)
{
    city_t *city;
    int shmd;
    
    /* Open the shared memory */
    shmd = shm_open(SHARED_MEM, O_RDWR, S_IRUSR | S_IWUSR);
    if (shmd == -1) {
        perror("Error when calling shm_open() from citizen_manager.\n");
        exit(EXIT_FAILURE);
    }

    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    
    //create_population(city);
    
    exit(EXIT_SUCCESS);
}
