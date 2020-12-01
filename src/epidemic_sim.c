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
 * @file epidemic_sim.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <mqueue.h>

#include "epidemic_sim.h"

int create_shared_memory()
{
    int mem;
    
    mem = shm_open(SHARED_MEM, O_CREAT | O_RDWR, S_IRWXU);
    if (mem < 0) {
        perror("Error when calling shm_open()\n");
        exit(EXIT_FAILURE);
    }
    
    if (ftruncate(mem, sizeof(city_t)) < 0) {
        perror("Error when calling ftruncate()\n");
        exit(EXIT_FAILURE);
    }
    
    return mem;
}

void generate_city(city_t *city) {
    city->map[0][7] = init_tile_firestation(0, 7);
    city->map[4][4] = init_tile_hospital(4, 4);
    city->map[7][0] = init_tile_firestation(7, 0);
}

/* mqd_t create_mqueue(); */

int main(void)
{
    int mem;
    /*mqd_t mqueue;*/

    city_t *city;
    
    srand(time(NULL));
    mem = create_shared_memory();
    /*mqueue = create_mqueue();*/

    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);
    generate_city(city);
    
    /* Début de la simulation */
    /* C'est à vous d'écrire là ... */
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    if (close(mem) < 0) {
        perror("Error when calling close(mem)\n");
    }
    if (shm_unlink(SHARED_MEM) < 0) {
        perror("Error when calling shm_unlink()\n");
    }
    exit(EXIT_SUCCESS);
}
