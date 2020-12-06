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
 * @version 1.0.0 - 2020-12-07
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
#include "fifo_messages.h"

pthread_t doctors[DOCTORS_NB];
pthread_t firemen[FIREMEN_NB];
pthread_t journalists[JOURNALISTS_NB];
pthread_t simple_citizens[SIMPLE_CITIZENS_NB];

int citizen_round;

void *doctor_process(void *status)
{
    status_t *st = (status_t *) status;

    int current_round = 0;
    
    printf("Je suis un docteur\n"); ///

    // TODO init 
    
    for(;;) {        
        if (citizen_round == -1) {
            //printf("C'est la fin !\n"); ///
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            //printf("Je soigne\n"); ///
        }
    }
    
    pthread_exit(NULL);
}

void *fireman_process(void *status)
{
    status_t *st = (status_t *) status;
    
    printf("Je suis un pompier\n"); ///
    
    pthread_exit(NULL);
}

void *journalist_process(void *status)
{
    status_t *st = (status_t *) status;
    
    printf("Je suis un journaliste\n"); ///
    
    pthread_exit(NULL);
}

void *simple_citizen_process(void *status)
{
    status_t *st = (status_t *) status;
    
    printf("Je suis un simple citoyen\n"); ///
    
    pthread_exit(NULL);
}

void init_population(city_t *city)
{
    int current_citizen;
    int i;

    citizen_round = 0;
    
    for (i = 0; i < DOCTORS_NB; i++) {
        pthread_create(&doctors[i], NULL, doctor_process,
                       (void*) &(city->citizens[current_citizen++]));
    }

    for (i = 0; i < FIREMEN_NB; i++) {
        pthread_create(&firemen[i], NULL, fireman_process,
                       (void*) &(city->citizens[current_citizen++]));
    }

    for (i = 0; i < JOURNALISTS_NB; i++) {
        pthread_create(&journalists[i], NULL, journalist_process,
                       (void*) &(city->citizens[current_citizen++]));
    }

    for (i = 0; i < SIMPLE_CITIZENS_NB; i++) {
        pthread_create(&simple_citizens[i], NULL, simple_citizen_process,
                       (void*) &(city->citizens[current_citizen++]));
    }
}

int main(void)
{
    int shared_memory;

    int fifo_from_epidemic_sim;
    
    city_t *city;
    
    fifo_message_e message_from_epidemic_sim[1];
    
    do {
        fifo_from_epidemic_sim = open(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, O_RDONLY);
    } while (fifo_from_epidemic_sim == -1);
    
    shared_memory = shm_open(SHARED_MEM, O_RDWR, S_IRUSR | S_IWUSR);
    if (shared_memory == -1) {
        perror("Error when calling shm_open() from citizen_manager\n");
        exit(EXIT_FAILURE);
    }
    
    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    
    init_population(city);

    for(;;) {
        read(fifo_from_epidemic_sim, message_from_epidemic_sim, sizeof(int));
        
        if (*message_from_epidemic_sim == END_OF_SIMULATION) {
            citizen_round = -1;
            break;
        }
        
        if (*message_from_epidemic_sim == NEXT_ROUND) {
            citizen_round++;
        }
    }
    
    close(fifo_from_epidemic_sim);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    exit(EXIT_SUCCESS);
}
