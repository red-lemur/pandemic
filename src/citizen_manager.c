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
 * @version 1.0.0 - 2020-12-05
 */

/**
 * @file citizen_manager.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "citizen_manager.h"
#include "epidemic_sim.h"

void create_population(city_t *city)
{
    
}

int main(void)
{
    int shared_memory;

    int fifo_from_epidemic_sim;
    
    city_t *city;

    char arr[10];
    
    fifo_from_epidemic_sim = open("/tmp/epidemic_sim_to_citizen_manager", O_RDONLY);
    
    read(fifo_from_epidemic_sim, arr, sizeof(arr));
    
    shared_memory = shm_open(SHARED_MEM, O_RDWR, S_IRUSR | S_IWUSR);
    if (shared_memory == -1) {
        perror("Error when calling shm_open() from citizen_manager\n");
        exit(EXIT_FAILURE);
    }
    
    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    printf("TYPE : %d\n", city->map[4][4].type);
    
    create_population(city);
    
    close(fifo_from_epidemic_sim);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    exit(EXIT_SUCCESS);
}
