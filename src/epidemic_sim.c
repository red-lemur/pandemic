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
 * @file epidemic_sim.c
 *
 * The most important process of the simulation. Manage the city.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "epidemic_sim.h"
#include "exchanges_between_processes.h"
#include "util.h"
#include "interface.h"
#include "file_paths.h"

struct sigaction action_sigusr1;
struct sigaction action_sigusr2;

int fifo_to_citizen_manager;
int fifo_to_press_agency;

fifo_message_e message_to_fifos[1];

int simulation_is_not_over = 1;

int round_nb = 0;

city_t *city;

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

void update_wastelands_contamination()
{
    int row;
    int col;

    int i;
    int j;
    
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_WIDTH; col++) {     
            if (city->map[col][row].type != WASTELAND) {
                continue;
            }
            
            for (i = row - 1; i <= row + 1; i++) {
                for (j = col - 1; j <= col + 1; j++) {
                    if (i < 0 || j < 0 || i >= CITY_HEIGHT || j >= CITY_WIDTH
                        || (i == row && j == col)) {
                        continue;
                    }
                    
                    if (city->map[j][i].type != WASTELAND
                        || city->map[col][row].contamination <= city->map[j][i].contamination) {
                        continue;
                    }
                    
                    if (generate_random_percentage() > PROBABILITY_WASTELAND_CONTAMINATION) {
                        continue;
                    }
                    
                    increase_wasteland_contamination(&(city->map[j][i]),
                                                     city->map[col][row].contamination);
                }
            }
        }   
    }
}

void increase_wasteland_contamination(tile_t *tile, double other_tile_contamination)
{
    double increase;
    double difference;
    
    increase = generate_random_percentage_in_interval(MIN_WASTELAND_CONTAMINATION_INCREASE,
                                                      MAX_WASTELAND_CONTAMINATION_INCREASE);
    difference = other_tile_contamination - tile->contamination;

    tile->contamination += increase*difference;
    
    if (tile->contamination > MAX_CONTAMINATION) {
        tile->contamination = MAX_CONTAMINATION;
    }
}

void hospitals_heal()
{
    int row;
    int col;
    
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_WIDTH; col++) {
            if (city->map[col][row].type == HOSPITAL) {
                hospital_heal_tile(&(city->map[col][row]));
            }   
        }
    }
}

void hospital_heal_tile(tile_t *tile)
{
    status_t *sickest;
    int healthy_doctors_nb_on_tile;
    int i;

    healthy_doctors_nb_on_tile = get_healthy_doctors_nb_on_tile(tile);
    
    for (i = 0; i < healthy_doctors_nb_on_tile; i++) {
        sickest = get_sickest_citizen_of_tile(tile);

        if (sickest == NULL) {
            return;
        }
        
        heal_citizen(sickest);
    }
}

int get_healthy_doctors_nb_on_tile(tile_t *tile)
{
    int counter;
    int i;

    counter = 0;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x == tile->x && city->citizens[i].y == tile->y
            && city->citizens[i].type == DOCTOR && !city->citizens[i].is_sick) {
            counter++;
        }
    }

    return counter;
}

status_t *get_sickest_citizen_of_tile(tile_t *tile)
{
    status_t *sickest;
    int i;
    
    sickest = NULL;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].x != tile->x || city->citizens[i].y != tile->y
            || !city->citizens[i].is_sick) {
            continue;
        }
        
        if (sickest == NULL) {
            sickest = &(city->citizens[i]);
        } else if (sickest->sickness_duration < city->citizens[i].sickness_duration) {
            sickest = &(city->citizens[i]);
        }
    }
    return sickest;
}

void heal_citizen(status_t *status)
{
    status->is_sick = 0;
    status->sickness_duration = 0;
    status->contamination = 0;
}

void launch_simulation()
{
    for(;;) {
        if (simulation_is_not_over) {
            pause();
        } else {
            break;
        }
    }
}

void simulation_round()
{
    ++round_nb;
    //printf("=====================================================\n"); ///
    //printf("Round %d\n", round_nb); ///
    
    save_evolution(round_nb);
    
    update_wastelands_contamination();

    /* DEBUG */
    /*int row, col;
    printf("=====================================================\n"); ///
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_WIDTH; col++) {
            printf("[%d %.3lf %d] ", city->map[col][row].type,
                   city->map[col][row].contamination, city->map[col][row].citizens_nb);
        }
        printf("\n");
    }
    int i;
    for (i = 0; i < CITIZENS_NB; i++) {
        printf("%d %d %.3lf T%d S%d tr:%d %s\n",
               city->citizens[i].x, city->citizens[i].y,
               city->citizens[i].contamination, city->citizens[i].type,
               city->citizens[i].is_sick,
               city->citizens[i].treatment_pouches_nb, city->citizens[i].name);
               }*/
    /* ----- */

    hospitals_heal();
    
    *message_to_fifos = NEXT_ROUND;
    //write(fifo_to_press_agency, message_to_fifos, sizeof(int));
    write(fifo_to_citizen_manager, message_to_fifos, sizeof(int));

    update_interface(round_nb, city);
}

void end_of_simulation()
{
    *message_to_fifos = END_OF_SIMULATION;
    write(fifo_to_press_agency, message_to_fifos, sizeof(int));
    write(fifo_to_citizen_manager, message_to_fifos, sizeof(int));
    
    simulation_is_not_over = 0;
}

void reset_evolution_file()
{
    FILE* file;
    file = fopen(EVOLUTION_URL, "w");
    fprintf(file, "# Tours | Personnes saines | Personnes malades | Personnes décédées | Cadavres brûlés\n");
    fclose(file);
}

void save_evolution(int round_nb)
{
    FILE* file = NULL;
    int burned_counter;
    int dead_counter;
    int sick_counter;
    int sane_counter;
    int i;

    burned_counter = 0;
    dead_counter = 0;
    sick_counter = 0;
    sane_counter = 0;
    for (i = 0; i < CITIZENS_NB; i++) {
        if (city->citizens[i].type == BURNED) {
            burned_counter++;
        } else if (city->citizens[i].type == DEAD) {
            dead_counter++;
        } else if (city->citizens[i].is_sick) {
            sick_counter++;
        } else {
            sane_counter++;
        }
    }
    
    file = fopen(EVOLUTION_URL, "a");    
    if (file != NULL) {
	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", round_nb, sane_counter, sick_counter,
                dead_counter, burned_counter);
	fclose(file);
    }
}

void open_fifos()
{
    unlink(FIFO_EPIDEMIC_SIM_TO_PRESS_AGENCY_URL);
    if (mkfifo(FIFO_EPIDEMIC_SIM_TO_PRESS_AGENCY_URL, S_IRUSR | S_IWUSR) == -1) {
        perror("Error while creating a FIFO to press_agency\n");
        exit(EXIT_FAILURE);
    }

    fifo_to_press_agency = open(FIFO_EPIDEMIC_SIM_TO_PRESS_AGENCY_URL, O_WRONLY);
    
    if (fifo_to_press_agency == -1) {
        perror("Error while opening a FIFO to press_agency\n");
        exit(EXIT_FAILURE);
    }
        
    unlink(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL);
    if (mkfifo(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, S_IRUSR | S_IWUSR) == -1) {
        perror("Error while creating a FIFO to citizen_manager\n");
        exit(EXIT_FAILURE);
    }

    fifo_to_citizen_manager = open(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, O_WRONLY);
    
    if (fifo_to_citizen_manager == -1) {
        perror("Error while opening a FIFO to citizen_manager\n");
        exit(EXIT_FAILURE);
    }
}

void close_fifos()
{
    close(fifo_to_press_agency);
    unlink(FIFO_EPIDEMIC_SIM_TO_PRESS_AGENCY_URL);
    
    close(fifo_to_citizen_manager);
    unlink(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL);
}

int main(void)
{
    int shared_memory;
    
    srand(time(NULL));
    
    shared_memory = create_shared_memory();

    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    load_map(city);
    
    action_sigusr1.sa_handler = &simulation_round;
    sigaction(SIGUSR1, &action_sigusr1, NULL);

    action_sigusr2.sa_handler = &end_of_simulation;
    sigaction(SIGUSR2, &action_sigusr2, NULL);

    open_fifos();
    
    create_interface(city);
    reset_evolution_file();
    launch_simulation();
    end_interface();

    close_fifos();
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    
    if (close(shared_memory) < 0) {
        perror("Error when calling close(mem)\n");
    }
    
    if (shm_unlink(SHARED_MEM) < 0) {
        perror("Error when calling shm_unlink()\n");
    }

    clear_terminal();

    if (execlp("gnuplot", "gnuplot", "-persist", COMMANDS_URL, (void*) 0) < 0) {
        printf("Error calling gnuplot\n");
        exit(EXIT_FAILURE);
    }
}
