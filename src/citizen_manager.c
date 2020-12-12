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
 * @version 1.0.0 - 2020-12-12
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
#include <time.h>

#include "citizen_manager.h"
#include "exchanges_between_processes.h"
#include "util.h"

city_t *city;

pthread_t doctors[DOCTORS_NB];
pthread_t firemen[FIREMEN_NB];
pthread_t journalists[JOURNALISTS_NB];
pthread_t simple_citizens[SIMPLE_CITIZENS_NB];

pthread_mutex_t mutex;

int citizen_round;

int init_doctors_in_hospital = 0;
int init_firemen_in_firestation = 0;

int citizen_ended_nb = 0;

void *doctor_process(void *status)
{
    status_t *st = (status_t *) status;

    int treatment_pouches_nb;
    
    int current_round;
    
    //printf("Je suis un docteur\n"); ///

    init_doctor(st, &treatment_pouches_nb);
    
    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
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

    double sprayer_capacity;

    int current_round;
    
    //printf("Je suis un pompier\n"); ///

    init_fireman(st, &sprayer_capacity);

    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            // BLABLA
        }
    }
    
    pthread_exit(NULL);
}

void *journalist_process(void *status)
{
    status_t *st = (status_t *) status;

    int current_round;
    
    //printf("Je suis un journaliste\n"); ///

    init_citizen(st, JOURNALIST);
    
    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            // BLABLA
        }
    }
    
    pthread_exit(NULL);
}

void *simple_citizen_process(void *status)
{
    status_t *st = (status_t *) status;

    int current_round;
    
    //printf("Je suis un simple citoyen\n"); ///

    init_citizen(st, SIMPLE_CITIZEN);

    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            // BLABLA
        }
    }
    
    pthread_exit(NULL);
}

void init_doctor(status_t *status, int* treatment_pouches_nb)
{
    int row;
    int col;

    int hospital_taken_counter;
    //printf("Je suis un docteur\n"); ///
    
    if (init_doctors_in_hospital < city->hospital_tiles_nb) {
        hospital_taken_counter = 0;
        
        for (row = 0; row < CITY_HEIGHT; row++) {
            for (col = 0; col < CITY_HEIGHT; col++) {
                if (city->map[col][row].type == HOSPITAL) {
                    if (!tile_is_full(city->map[col][row])
                        && hospital_taken_counter == init_doctors_in_hospital) {
                        add_citizen_in_tile(&(city->map[col][row]));
                        init_citizen_status(status, col, row, DOCTOR);
                        *treatment_pouches_nb = MAX_TREATMENT_POUCHES_NB;
                        increment_init_doctors_in_hospital();
                        return;
                    }
                    hospital_taken_counter++;
                }
            }
        }
    }
    
    init_citizen(status, DOCTOR);
    
    *treatment_pouches_nb = TREATMENT_POUCHES_NB_AT_BEGINNING;

    //printf("Je suis un docteur et END\n"); ///
}

void init_fireman(status_t *status, double *sprayer_capacity)
{
    int row;
    int col;
    
    int firestation_taken_counter;
    //printf("Je suis un pompier\n"); ///
    
    if (init_firemen_in_firestation < city->firestation_tiles_nb) {
        firestation_taken_counter = 0;
        
        for (row = 0; row < CITY_HEIGHT; row++) {
            for (col = 0; col < CITY_HEIGHT; col++) {
                if (city->map[col][row].type == FIRESTATION) {
                    if (!tile_is_full(city->map[col][row])
                        && firestation_taken_counter == init_firemen_in_firestation) {
                        add_citizen_in_tile(&(city->map[col][row]));
                        init_citizen_status(status, col, row, FIREMAN);
                        *sprayer_capacity = MAX_SPRAYER_CAPACITY;
                        increment_init_firemen_in_firestation();
                        return;
                    }
                    firestation_taken_counter++;
                }
            }
        }
    }
    
    init_citizen(status, FIREMAN);
    
    *sprayer_capacity = SPRAYER_CAPACITY_AT_BEGINNING;

    //printf("Je suis un pompier et END\n"); ///
}

void init_citizen(status_t *status, citizen_type_e type)
{
    int row;
    int col;
    
    do {
        row = generate_random_index(CITY_HEIGHT);
        col = generate_random_index(CITY_WIDTH);
    } while (tile_is_full(city->map[col][row])
             || (type != FIREMAN && firemen_nb_in_firestation(row, col) == 0)
             || (city->map[col][row].type == HOSPITAL
                 && !is_allowed_to_enter_in_a_hospital(status)));
    
    add_citizen_in_tile(&(city->map[col][row]));
    init_citizen_status(status, col, row, type);
}

void init_citizen_status(status_t *status, int x, int y, citizen_type_e type)
{
    pthread_mutex_lock(&mutex);
    status->x = x;
    status->y = y;
    status->contamination = 0;
    status->is_sick = 0;
    status->sickness_duration = 0;
    status->type = type;
    status->days_spent_in_hospital_healthy = 0;
    pthread_mutex_unlock(&mutex);
}

void add_citizen_in_tile(tile_t *tile)
{
    pthread_mutex_lock(&mutex);
    tile->citizens_nb++;
    pthread_mutex_unlock(&mutex);
}

void increment_init_doctors_in_hospital()
{
    pthread_mutex_lock(&mutex);
    init_doctors_in_hospital++;
    pthread_mutex_unlock(&mutex);
}

void increment_init_firemen_in_firestation()
{
    pthread_mutex_lock(&mutex);
    init_firemen_in_firestation++;
    pthread_mutex_unlock(&mutex);
}

int firemen_nb_in_firestation(unsigned int row, unsigned int col)
{
    int counter;
    int i;
    
    if (city->map[col][row].type != FIRESTATION) {
        return -1;
    }
    
    counter = 0;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x == col && city->citizens[i].y == row
            && city->citizens[i].type == FIREMAN) {
            counter++;
        }
    }
    
    return counter;
}

int visitors_nb_in_firestation(unsigned int row, unsigned int col)
{
    int counter;
    int i;
    
    if (city->map[col][row].type != FIRESTATION) {
        return -1;
    }

    counter = 0;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x == col && city->citizens[i].y == row
            && city->citizens[i].type != FIREMAN) {
            counter++;
        }
    }
    
    return counter;
}

int is_allowed_to_enter_in_a_hospital(status_t *status) {
    return status->type == DOCTOR || status->type == FIREMAN || status->is_sick;
}

void move_citizen()
{
    pthread_mutex_lock(&mutex);
    
    // Blabla
    
    pthread_mutex_unlock(&mutex);
}

void init_population()
{
    int current_citizen;
    int i;

    pthread_mutex_init(&mutex, NULL);
    
    citizen_round = 0;
    current_citizen = 0;
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

void citizen_ended()
{
    pthread_mutex_lock(&mutex);
    citizen_ended_nb++;
    pthread_mutex_unlock(&mutex);
}

int tile_is_full(tile_t tile)
{
    return tile.citizens_nb == tile.capacity;
}

int main(void)
{
    int shared_memory;

    int fifo_from_epidemic_sim;
    
    fifo_message_e message_from_epidemic_sim[1];

    srand(time(NULL));
    
    do {
        fifo_from_epidemic_sim = open(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, O_RDONLY);
    } while (fifo_from_epidemic_sim == -1);
    
    shared_memory = shm_open(SHARED_MEM, O_RDWR, S_IRUSR | S_IWUSR);
    if (shared_memory == -1) {
        perror("Error when calling shm_open() from citizen_manager\n");
        exit(EXIT_FAILURE);
    }
    
    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    
    init_population();

    for (;;) {
        read(fifo_from_epidemic_sim, message_from_epidemic_sim, sizeof(int));
        
        if (*message_from_epidemic_sim == END_OF_SIMULATION) {
            citizen_round = -1;
                
            break;
        }
        
        if (*message_from_epidemic_sim == NEXT_ROUND) {
            citizen_round++;
        }
    }

    for (;;) {
        if (citizen_ended_nb == CITIZENS_NB) {
            break;
        }
    }
    
    close(fifo_from_epidemic_sim);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    exit(EXIT_SUCCESS);
}
