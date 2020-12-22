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
 * @version 1.0.0 - 2020-12-22
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

mqd_t mq;
struct mq_attr attr;

city_t *city;

pthread_t doctors[DOCTORS_NB];
pthread_t firemen[FIREMEN_NB];
pthread_t journalists[JOURNALISTS_NB];
pthread_t simple_citizens[SIMPLE_CITIZENS_NB];

pthread_mutex_t mutex;

int citizen_round;

unsigned int id_generator = 0;
int init_doctors_in_hospital = 0;
int init_firemen_in_firestation = 0;
int citizen_ended_nb = 0;

void *doctor_process(void *status)
{
    status_t *st = (status_t *) status;
    int current_round;
    
    init_doctor(st);
    
    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            every_citizen_round(st);
        }
    }
    
    pthread_exit(NULL);
}

void *fireman_process(void *status)
{
    status_t *st = (status_t *) status;
    int current_round;
    
    init_fireman(st);

    current_round = 0;
    for(;;) {        
        if (citizen_round == -1) {
            citizen_ended();
            break;
        }

        if (current_round < citizen_round) {
            current_round++;
            every_citizen_round(st);
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
            every_citizen_round(st);
            send_news(st);
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
            every_citizen_round(st);
        }
    }
    
    pthread_exit(NULL);
}

mqd_t open_mqueue()
{
    mqd_t mq;
    
    mq = mq_open(MQUEUE, O_WRONLY | O_NONBLOCK, 0600, NULL);
    if (mq == (mqd_t) -1) {
        perror("Error when calling mq_open()\n");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mq, &attr) != 0) {
        perror("Error when calling mq_getattr()\n");
        exit(EXIT_FAILURE);
    }
    
    return mq;
}

void send_news(status_t *status)
{
    char buffer[MAX_MESSAGES_SIZE];
    
    sprintf(buffer, "Nombre total de citoyens contaminés : %d", city->citizens_sick_number);
    mq_send(mq, buffer, sizeof(buffer), CITIZENS_CONTAMINATION_PRIORITY);
    
    sprintf(buffer, "Niveau moyen de contamination de la ville : %.3lf",
            city->city_mean_contamination);
    mq_send(mq, buffer, sizeof(buffer), CITY_CONTAMINATION_PRIORITY);
    
    sprintf(buffer, "Nombre total de morts : %d", city->deads_number);
    mq_send(mq, buffer, sizeof(buffer), DEADS_NUMBER_PRIORITY);
    
    sprintf(buffer, "Taux de contamination du journaliste %s : %.3lf", status->name,
            status->contamination);
    mq_send(mq, buffer, sizeof(buffer), PERSONNAL_CONTAMINATION_PRIORITY);
}

void every_citizen_round(status_t *status)
{
    tile_decrease_citizen_contamination(status);
    sprayer_decontamination(status);
    doctor_can_heal(status);
    increment_sickness_duration(status);
    increase_tile_contamination(status);
    contaminate_other_citizens_if_sick(status);
    move_citizen(status);
    risk_of_death(status);
    risk_of_disease(status);
}

void init_doctor(status_t *status)
{
    int row;
    int col;

    int hospital_taken_counter;
    
    if (init_doctors_in_hospital < city->hospital_tiles_nb) {
        increment_init_doctors_in_hospital();
        hospital_taken_counter = 0;
        
        for (row = 0; row < CITY_HEIGHT; row++) {
            for (col = 0; col < CITY_HEIGHT; col++) {
                if (city->map[col][row].type == HOSPITAL) {
                    if (!tile_is_full(&(city->map[col][row]))
                        && hospital_taken_counter == init_doctors_in_hospital - 1) {
                        add_citizen_in_tile(&(city->map[col][row]));
                        init_citizen_status(status, col, row, DOCTOR);
                        refill_treatment_pouches(status);
                        return;
                    }
                    hospital_taken_counter++;
                }
            }
        }
    }
    
    init_citizen(status, DOCTOR);

    pthread_mutex_lock(&mutex);
    status->treatment_pouches_nb = TREATMENT_POUCHES_NB_AT_BEGINNING;
    pthread_mutex_unlock(&mutex);
}

void init_fireman(status_t *status)
{
    int row;
    int col;
    
    int firestation_taken_counter;
    
    if (init_firemen_in_firestation < city->firestation_tiles_nb) {
        increment_init_firemen_in_firestation();
        firestation_taken_counter = 0;
        
        for (row = 0; row < CITY_HEIGHT; row++) {
            for (col = 0; col < CITY_HEIGHT; col++) {
                if (city->map[col][row].type == FIRESTATION) {
                    if (!tile_is_full(&(city->map[col][row]))
                        && firestation_taken_counter == init_firemen_in_firestation - 1) {
                        add_citizen_in_tile(&(city->map[col][row]));
                        init_citizen_status(status, col, row, FIREMAN);
                        refill_sprayer(status);
                        return;
                    }
                    firestation_taken_counter++;
                }
            }
        }
    }
    
    init_citizen(status, FIREMAN);

    pthread_mutex_lock(&mutex);
    status->sprayer_capacity = SPRAYER_CAPACITY_AT_BEGINNING;
    pthread_mutex_unlock(&mutex);
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
    status->id = id_generator++;
    strcpy(status->name, CITIZEN_NAMES[generate_random_index(NAMES_NB)]);
    status->x = x;
    status->y = y;
    status->contamination = 0;
    status->is_sick = 0;
    status->sickness_duration = 0;
    status->type = type;
    status->days_spent_in_hospital_healthy = 0;
    status->must_leave = 0;
    status->doctor_can_enter_hospital = 0;
    status->treatment_pouches_nb = 0;
    status->sprayer_capacity = 0;
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

void refill_treatment_pouches(status_t *status)
{
    pthread_mutex_lock(&mutex);
    status->treatment_pouches_nb = MAX_TREATMENT_POUCHES_NB;
    pthread_mutex_unlock(&mutex);
}

void refill_sprayer(status_t *status)
{
    pthread_mutex_lock(&mutex);
    status->sprayer_capacity = MAX_SPRAYER_CAPACITY;
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
    status->must_leave = 0;
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

int doctor_nb_in_tile(tile_t* tile)
{
    int counter;
    int i;
    
    counter = 0;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x == tile->x && city->citizens[i].y == tile->y
            && city->citizens[i].type == DOCTOR) {
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
    if (status->type == DEAD || status->type == BURNED) {
        return;
    }

    if (status->doctor_can_enter_hospital > 0) {
        pthread_mutex_lock(&mutex);
        status->doctor_can_enter_hospital--;
        pthread_mutex_unlock(&mutex);
    }
    
    for (;;) {
        if (citizen_can_leave_tile(status)) {
            if (citizen_try_to_leave_tile(status)) {
                return;
            }
        } else {
            if (city->map[status->x][status->y].type == HOSPITAL
                && !status->is_sick
                && (status->type == SIMPLE_CITIZEN || status->type == JOURNALIST)) {
                pthread_mutex_lock(&mutex);
                status->days_spent_in_hospital_healthy++;
                pthread_mutex_unlock(&mutex);
            }
            
            if (status->days_spent_in_hospital_healthy == MAX_DAYS_IN_HOSPITAL_HEALTHY) {
                pthread_mutex_lock(&mutex);
                status->must_leave = 1;
                status->days_spent_in_hospital_healthy = 0;
                pthread_mutex_unlock(&mutex);
            }
            
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
            || is_allowed_to_enter_in_a_hospital(status))
        && status->doctor_can_enter_hospital == 0;
}

int citizen_try_to_leave_tile(status_t *status)
{
    unsigned int new_x;
    unsigned int new_y;
    
    generate_new_citizen_position(status, &new_x, &new_y);
    
    if (citizen_can_enter_tile(status, &(city->map[new_x][new_y]))) {
        remove_citizen_from_tile(&(city->map[status->x][status->y]));
        add_citizen_in_tile(&(city->map[new_x][new_y]));
        
        if (city->map[status->x][status->y].type == HOSPITAL
            && status->type == DOCTOR) {
            pthread_mutex_lock(&mutex);
            status->doctor_can_enter_hospital = DAYS_NB_DOCTOR_HAS_TO_WAIT;
            pthread_mutex_unlock(&mutex);
        }
        
        update_citizen_coords(status, new_x, new_y);
        increase_citizen_contamination(status, MOVE);
        
        if (status->must_leave) {
            pthread_mutex_lock(&mutex);
            status->must_leave = 0;
            pthread_mutex_unlock(&mutex);
        }
        
        if (city->map[status->x][status->y].type == HOSPITAL
            && status->type == DOCTOR) {
            refill_treatment_pouches(status);
        }
        
        if (city->map[status->x][status->y].type == FIRESTATION
            && status->type == FIREMAN) {
            refill_sprayer(status);
        }
        
        burn_the_dead(status);
        return 1;
    } else if (status->type != DEAD && status->is_sick
               && city->map[new_x][new_y].type == HOSPITAL
               && tile_is_full(&(city->map[new_x][new_y]))) {
        healed_citizens_must_leave_hospital(&(city->map[new_x][new_y]));
    }
    return 0;
}

void healed_citizens_must_leave_hospital(tile_t* tile)
{
    int i;
    
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x == tile->x && city->citizens[i].y == tile->y
            && (city->citizens[i].type == JOURNALIST
                || city->citizens[i].type == SIMPLE_CITIZEN)
            && !city->citizens[i].is_sick) {
            pthread_mutex_lock(&mutex);
            city->citizens[i].must_leave = 1;
            pthread_mutex_unlock(&mutex);
        }
    }
}

void generate_new_citizen_position(status_t *status, unsigned int* new_x, unsigned int* new_y)
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;

    min_x = (status->x == 0) ? 0 : -1;
    min_y = (status->y == 0) ? 0 : -1;
    max_x = (status->x == CITY_WIDTH - 1) ? 0 : 1;
    max_y = (status->y == CITY_HEIGHT - 1) ? 0 : 1;

    do {
        *new_x = generate_random_int_in_interval(status->x + min_x, status->x + max_x);
        *new_y = generate_random_int_in_interval(status->y + min_y, status->y + max_y);
    } while (*new_x == status->x && *new_y == status->y);
}

void tile_decrease_citizen_contamination(status_t *status)
{
    if (status->type == DEAD || status->type == BURNED) {
        return;
    }

    pthread_mutex_lock(&mutex);
    if (city->map[status->x][status->y].type == FIRESTATION) {
        status->contamination -= CONTAMINATION_DECREASE_IN_FIRESTATION;
    }
    
    if (city->map[status->x][status->y].type == HOSPITAL
        && status->contamination > city->map[status->x][status->y].contamination) {
        status->contamination -= CONTAMINATION_DECREASE_IN_HOSPITAL;
        if (status->contamination < city->map[status->x][status->y].contamination) {
            status->contamination = city->map[status->x][status->y].contamination;
        }
        
    }

    if (status->contamination < MIN_CONTAMINATION) {
        status->contamination = MIN_CONTAMINATION;
    }
    pthread_mutex_unlock(&mutex);
}

void sprayer_decontamination(status_t *status)
{
    status_t *most_contaminated;
    double sprayer_used;
    
    if (status->type != FIREMAN || city->map[status->x][status->y].type == FIRESTATION) {
        return;
    }

    sprayer_used = 0;
    do {
        most_contaminated
            = get_most_contaminated_citizen_of_tile(status, &(city->map[status->x][status->y]));
        
        if (most_contaminated == NULL) {
            if (city->map[status->x][status->y].contamination == 0) {
                break;
            }
            
            if (city->map[status->x][status->y].contamination < MAX_SPRAYER_DECONTAMINATION) {
                decontamine_tile_with_sprayer(status, &(city->map[status->x][status->y]),
                                              city->map[status->x][status->y].contamination);
                sprayer_used += city->map[status->x][status->y].contamination;
            } else {
                decontamine_tile_with_sprayer(status, &(city->map[status->x][status->y]),
                                              MAX_SPRAYER_DECONTAMINATION);
                sprayer_used += MAX_SPRAYER_DECONTAMINATION;
            }
        } else {
            if (most_contaminated->contamination < MAX_SPRAYER_DECONTAMINATION) {
                decontamine_citizen_with_sprayer(status, most_contaminated,
                                                 most_contaminated->contamination);
                sprayer_used += most_contaminated->contamination;
            } else {
                decontamine_citizen_with_sprayer(status, most_contaminated,
                                                 MAX_SPRAYER_DECONTAMINATION);
                sprayer_used += MAX_SPRAYER_DECONTAMINATION;
            }
        }
    }
    while (sprayer_used < MAX_SPRAYER_USE && status->sprayer_capacity > 0
           && !(most_contaminated == NULL
                && city->map[status->x][status->y].contamination == 0));
}

status_t *get_most_contaminated_citizen_of_tile(status_t *fireman_status, tile_t *tile)
{
    int i;
    status_t *most_contaminated;

    most_contaminated = NULL;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x != tile->x || city->citizens[i].y != tile->y
            || city->citizens[i].id == fireman_status->id) {
            continue;
        }
        
        if (most_contaminated == NULL) {
            if (city->citizens[i].contamination > 0) {
                most_contaminated = &(city->citizens[i]);
            }
        } else if (most_contaminated->contamination < city->citizens[i].contamination) {
            most_contaminated = &(city->citizens[i]);
        }
    }
    return most_contaminated;
}

void decontamine_tile_with_sprayer(status_t *status, tile_t *tile, double decontamination)
{
    if (status->sprayer_capacity < decontamination) {
        decontamination = status->sprayer_capacity;
    }
    
    pthread_mutex_lock(&mutex);
    tile->contamination -= decontamination;
    status->sprayer_capacity -= decontamination;
    pthread_mutex_unlock(&mutex);
}

void decontamine_citizen_with_sprayer(status_t *fireman_status, status_t *citizen_status,
                                      double decontamination)
{
    if (fireman_status->sprayer_capacity < decontamination) {
        decontamination = fireman_status->sprayer_capacity;
    }
    
    pthread_mutex_lock(&mutex);
    citizen_status->contamination -= decontamination;
    fireman_status->sprayer_capacity -= decontamination;
    pthread_mutex_unlock(&mutex);
}

void doctor_can_heal(status_t *status)
{
    status_t *sickest;
    
    if (status->type != DOCTOR || city->map[status->x][status->y].type == HOSPITAL
        || status->treatment_pouches_nb == 0
        || status->sickness_duration >= DAYS_NB_SICK_DOCTOR_CAN_HEAL_HIMSELF) {
        return;
    }
    
    if (status->is_sick) {
        heal(status, status);
        return;
    }

    sickest = get_sickest_citizen_of_tile(&(city->map[status->x][status->y]));
    if (sickest != NULL) {
        heal(status, sickest);
    }
}

void heal(status_t *doctor_status, status_t *citizen_status)
{
    pthread_mutex_lock(&mutex);
    doctor_status->treatment_pouches_nb--;
    citizen_status->is_sick = 0;
    citizen_status->sickness_duration = 0;
    citizen_status->contamination = 0;
    pthread_mutex_unlock(&mutex);
}

status_t *get_sickest_citizen_of_tile(tile_t *tile)
{
    int i;
    status_t *sickest;

    sickest = NULL;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x != tile->x || city->citizens[i].y != tile->y
            || !city->citizens[i].is_sick) {
            continue;
        }
        
        if (sickest == NULL) {
            sickest = &(city->citizens[i]);
        } else if (sickest->contamination < city->citizens[i].contamination) {
            sickest = &(city->citizens[i]);
        }
    }
    return sickest;
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

void contaminate_other_citizens_if_sick(status_t *status)
{
    if (!status->is_sick) {
        return;
    }

    if (generate_random_percentage() <= PROB_CONTAMINATE_IF_SICK_TILE) {
        contaminate_citizens_in_same_tile(status);
    }
    
    if (generate_random_percentage() <= PROB_CONTAMINATE_IF_SICK_WASTELANDS_AROUND) {
        contaminate_citizens_in_wastelands_around(status);
    }
}

void contaminate_citizens_in_same_tile(status_t *status)
{
    int i;
    
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x != status->x
            || city->citizens[i].y != status->y
            || city->citizens[i].id == status->id) {
            continue;
        }
        
        if (city->citizens[i].type == FIREMAN
            && generate_random_percentage() <= FIREMAN_PROTECTION_CITIZEN) {
            continue;
        }
        
        pthread_mutex_lock(&mutex);
        
        city->citizens[i].contamination += status->contamination
            * CONTAMINATION_CITIZEN_BY_CITIZEN;
        
        if (city->citizens[i].contamination > MAX_CONTAMINATION) {
            city->citizens[i].contamination = MAX_CONTAMINATION;
        }
        
        pthread_mutex_unlock(&mutex);
    }
}

void contaminate_citizens_in_wastelands_around(status_t *status)
{
    int i;
    unsigned int other_x;
    unsigned int other_y;

    if (city->map[status->x][status->y].type != WASTELAND) {
        return;
    }
    
    for (i = 0; i < CITIZENS_NB; i++) {
        other_x = city->citizens[i].x;
        other_y = city->citizens[i].y;
        
        if (city->map[other_x][other_y].type != WASTELAND
            || other_x < status->x - 1 || other_x > status->y + 1
            || other_y < status->y - 1 || other_y > status->y + 1
            || (other_x == status->x && other_y == status->y)) {
            continue;
        }
        
        if (city->citizens[i].type == FIREMAN
            && generate_random_percentage() <= FIREMAN_PROTECTION_CITIZEN) {
            continue;
        }
        
        pthread_mutex_lock(&mutex);
        
        city->citizens[i].contamination += status->contamination
            * CONTAMINATION_CITIZEN_BY_CITIZEN;
        
        if (city->citizens[i].contamination > MAX_CONTAMINATION) {
            city->citizens[i].contamination = MAX_CONTAMINATION;
        }
        
        pthread_mutex_unlock(&mutex);
    }
}

void burn_the_dead(status_t *status)
{
    int i;
    
    if (status->type != FIREMAN) {
        return;
    }
    
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x == status->x
            && city->citizens[i].y == status->y
            && city->citizens[i].type == DEAD) {
            pthread_mutex_lock(&mutex);
            city->citizens[i].type = BURNED;
            city->citizens[i].is_sick = 0;
            pthread_mutex_unlock(&mutex);
        }
    }
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
    double death_reduction;
    
    if (status->type == DEAD || status->type == BURNED
        || !status->is_sick || status->sickness_duration < DAYS_NB_DISEASE_DEADLY) {
        return;
    }
    
    if (city->map[status->x][status->y].type == HOSPITAL) {
        death_reduction = DEATH_REDUCTION_IF_HOSPITAL;
    } else if (doctor_nb_in_tile(&(city->map[status->x][status->y]))) {
        death_reduction = DEATH_REDUCTION_IF_DOCTOR;
    }
    
    if (generate_random_percentage() <= death_reduction
        * (status->sickness_duration - DAYS_NB_DISEASE_DEADLY + 1) * PROB_OF_DEATH_BY_DAY) {
        pthread_mutex_lock(&mutex);
        status->type = DEAD;
        pthread_mutex_unlock(&mutex);
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

    mq = open_mqueue();
    
    init_population();

    citizens_simulation();
    
    wait_for_citizens_to_end();

    if (mq_close(mq) < 0) {
        perror("Error when calling mq_close(mq)\n");
    }
    
    close(fifo_from_epidemic_sim);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    exit(EXIT_SUCCESS);
}
