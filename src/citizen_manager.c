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
#include <pthread.h>

#include "citizen_manager.h"
#include "epidemic_sim.h"

void *doctor_process(void *vargp) { // A CHANGER
    printf("Salut\n");
    
    pthread_exit(NULL);
}

void init_population(city_t *city)
{
    int i;
    
    for (i = 0; i < DOCTORS_NB; i++) {
        pthread_create(&doctors[i], NULL, doctor_process, NULL);
    }
    
    /*for (i = 0; i < FIREMEN_NB; i++) {
        pthread_create(..., fireman_process, ...);
    }
    ...*/
}

int main(void)
{
    int shared_memory;

    int fifo_from_epidemic_sim;
    
    city_t *city;

    //char arr[10];
    
    /*fifo_from_epidemic_sim = open(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, O_RDONLY);

    if (fifo_from_epidemic_sim == -1) {
        perror("Error while opening a FIFO\n");
        exit(EXIT_FAILURE);
        }*/

    do {
        fifo_from_epidemic_sim = open(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, O_RDONLY);
    } while (fifo_from_epidemic_sim == -1);
    
    //read(fifo_from_epidemic_sim, arr, sizeof(arr));
    
    shared_memory = shm_open(SHARED_MEM, O_RDWR, S_IRUSR | S_IWUSR);
    if (shared_memory == -1) {
        perror("Error when calling shm_open() from citizen_manager\n");
        exit(EXIT_FAILURE);
    }
    
    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    //printf("TYPE : %d\n", city->map[0][6].type);///
    
    init_population(city);
    
    close(fifo_from_epidemic_sim);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    exit(EXIT_SUCCESS);
}
