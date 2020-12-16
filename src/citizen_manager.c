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
 * @version 1.0.0 - 2020-12-16
 */

/**
 * @file citizen_manager.c
 *
 * Manage the citizens of the city.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#include "citizen_manager.h"
#include "exchanges_between_processes.h"
#include "util.h"
#include "city_sim.h"
#include "names.h"

int fifo_from_epidemic_sim;

fifo_message_e message_from_epidemic_sim[1];

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
    
    init_doctor(st, &treatment_pouches_nb);
    
    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            increment_sickness_duration(st);
            increase_tile_contamination(st);
            move_citizen(st);
            risk_of_death(st);
            risk_of_disease(st);
        }
    }
    
    pthread_exit(NULL);
}

void *fireman_process(void *status)
{
    status_t *st = (status_t *) status;

    double sprayer_capacity;

    int current_round;
    
    init_fireman(st, &sprayer_capacity);

    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            increment_sickness_duration(st);
            increase_tile_contamination(st);
            move_citizen(st);
            risk_of_death(st);
            risk_of_disease(st);
        }
    }
    
    pthread_exit(NULL);
}

void *journalist_process(void *status)
{
    status_t *st = (status_t *) status;

    int current_round;
    
    init_citizen(st, JOURNALIST);
    
    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            increment_sickness_duration(st);
            increase_tile_contamination(st);
            move_citizen(st);
            risk_of_death(st);
            risk_of_disease(st);
        }
    }
    
    pthread_exit(NULL);
}

void *simple_citizen_process(void *status)
{
    status_t *st = (status_t *) status;

    int current_round;

    init_citizen(st, SIMPLE_CITIZEN);

    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            increment_sickness_duration(st);
            increase_tile_contamination(st);
            move_citizen(st);
            risk_of_death(st);
            risk_of_disease(st);
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
                    if (!tile_is_full(&(city->map[col][row]))
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
                    if (!tile_is_full(&(city->map[col][row]))
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
    } while (tile_is_full(&(city->map[col][row]))
             || (type != FIREMAN && firemen_nb_in_firestation(col, row) == 0)
             || (city->map[col][row].type == HOSPITAL
                 && !is_allowed_to_enter_in_a_hospital(status)));
    
    add_citizen_in_tile(&(city->map[col][row]));
    init_citizen_status(status, col, row, type);
}

void init_citizen_status(status_t *status, int x, int y, citizen_type_e type)
{
    pthread_mutex_lock(&mutex);
    strcpy(status->name, CITIZEN_NAMES[generate_random_index(NAMES_NB)]);
    status->x = x;
    status->y = y;
    status->contamination = 0;
    status->is_sick = 0;
    status->sickness_duration = 0;
    status->type = type;
    status->days_spent_in_hospital_healthy = 0;
    status->must_leave = 0;
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

void add_citizen_in_tile(tile_t *tile)
{
    pthread_mutex_lock(&mutex);
    tile->citizens_nb++;
    pthread_mutex_unlock(&mutex);
}

void remove_citizen_from_tile(tile_t *tile)
{
    pthread_mutex_lock(&mutex);
    tile->citizens_nb--;
    pthread_mutex_unlock(&mutex);
}

void update_citizen_coords(status_t *status, unsigned int x, unsigned int y)
{
    pthread_mutex_lock(&mutex);
    status->x = x;
    status->y = y;
    pthread_mutex_unlock(&mutex);
}

int firemen_nb_in_firestation(unsigned int col, unsigned int row)
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

int visitors_nb_in_firestation(unsigned int col, unsigned int row)
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

int is_allowed_to_enter_in_a_hospital(status_t *status)
{
    return status->type == DOCTOR || status->type == FIREMAN || status->is_sick;
}

void move_citizen(status_t *status)
{
    int new_x;
    int new_y;
    
    if (status->type == DEAD || status->type == BURNED) {
        return;
    }
    
    for (;;) {
        if (citizen_can_leave_tile(status)) {
            generate_new_citizen_position(status, &new_x, &new_y);
            
            if (citizen_can_enter_tile(status, &(city->map[new_x][new_y]))) {
                remove_citizen_from_tile(&(city->map[status->x][status->y]));
                add_citizen_in_tile(&(city->map[new_x][new_y]));
                
                update_citizen_coords(status, new_x, new_y);
                increase_citizen_contamination(status, MOVE);
                return;
            }
        } else {
            increase_citizen_contamination(status, STAY);
            return;
        }
    }
}

int citizen_can_leave_tile(status_t *status)
{
    return status->must_leave
        || ((status->type != FIREMAN
             || (visitors_nb_in_firestation(status->x, status->y) <= 0
                 || firemen_nb_in_firestation(status->x, status->y) > 1))
            && generate_random_percentage() > PROB_TO_STAY_ON_TILE);
}

int citizen_can_enter_tile(status_t *status, tile_t *tile)
{
    return !tile_is_full(tile)
        && (status->type == FIREMAN
            || firemen_nb_in_firestation(tile->x, tile->y) == -1
            || firemen_nb_in_firestation(tile->x, tile->y) > 0)
        && (city->map[tile->x][tile->y].type != HOSPITAL
            || is_allowed_to_enter_in_a_hospital(status));
}

void generate_new_citizen_position(status_t *status, int* new_x, int* new_y)
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;

    min_x = status->x == 0 ? 0 : -1;
    min_y = status->y == 0 ? 0 : -1;
    max_x = status->x == CITY_WIDTH - 1 ? 0 : 1;
    max_y = status->y == CITY_HEIGHT - 1 ? 0 : 1;
    *new_x = status->x + generate_random_int_in_interval(min_x, max_x);
    *new_y = status->y + generate_random_int_in_interval(min_y, max_y);
}

void increase_tile_contamination(status_t *status)
{
    unsigned int x;
    unsigned int y;
    
    double contamination_reduction;

    x = status->x;
    y = status->y;
    
    if (status->type == BURNED || city->map[x][y].type == FIRESTATION) {
        return;
    }
    
    pthread_mutex_lock(&mutex);
    
    contamination_reduction = 1;
    if (city->map[x][y].type == HOSPITAL) {
        contamination_reduction = HOSPITAL_CONTAMINATION_REDUCTION;
    }
    
    city->map[x][y].contamination += status->contamination
        * CONTAMINATION_INCREASE_TILE * contamination_reduction;
    
    if (city->map[x][y].contamination > MAX_CONTAMINATION) {
        city->map[x][y].contamination = MAX_CONTAMINATION;
    }
    
    pthread_mutex_unlock(&mutex);
}

void increase_citizen_contamination(status_t *status, int move)
{
    unsigned int x;
    unsigned int y;

    double protection_factor;

    x = status->x;
    y = status->y;

    protection_factor = 1;
    if (status->type == FIREMAN) {
        protection_factor = FIREMAN_PROTECTION_FACTOR;
    }

    pthread_mutex_lock(&mutex);
    
    if (move == MOVE) {
        status->contamination += city->map[x][y].contamination
            * CONTAMINATION_INCREASE_MOVE_CITIZEN * protection_factor;
    } else {
        status->contamination += city->map[x][y].contamination
            * CONTAMINATION_INCREASE_STAY_CITIZEN * protection_factor;
    }
    
    if (status->contamination > MAX_CONTAMINATION) {
        status->contamination = MAX_CONTAMINATION;
    }
    
    pthread_mutex_unlock(&mutex);
}

void risk_of_disease(status_t *status)
{
    if (status->type == BURNED || status->is_sick) {
        return;
    }
    
    if (generate_random_percentage() <= status->contamination) {
        pthread_mutex_lock(&mutex);
        status->is_sick = 1;
        pthread_mutex_unlock(&mutex);
        
    }
}

void increment_sickness_duration(status_t *status)
{
    if (status->is_sick) {
        pthread_mutex_lock(&mutex);
        status->sickness_duration++;
        pthread_mutex_unlock(&mutex);
    }
}

void risk_of_death(status_t *status)
{
    if (status->type == DEAD || status->type == BURNED
        || !status->is_sick || status->sickness_duration < DAYS_NB_DISEASE_DEADLY) {
        return;
    }
    
    if (generate_random_percentage() <=
        (status->sickness_duration - DAYS_NB_DISEASE_DEADLY + 1) * PROB_OF_DEATH_BY_DAY) {
        pthread_mutex_lock(&mutex);
        status->type = DEAD;
        pthread_mutex_unlock(&mutex);
        printf("=====> %s    %d\n", status->name, status->sickness_duration);
    }
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

int tile_is_full(tile_t *tile)
{
    return tile->citizens_nb == tile->capacity;
}

void citizens_simulation()
{
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
}

void wait_for_citizens_to_end()
{
    for (;;) {
        if (citizen_ended_nb == CITIZENS_NB) {
            break;
        }
    }
}

int main(void)
{
    int shared_memory;
    
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

    citizens_simulation();

    wait_for_citizens_to_end();
    
    close(fifo_from_epidemic_sim);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    exit(EXIT_SUCCESS);
}
