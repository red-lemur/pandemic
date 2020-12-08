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
 * @version 1.0.0 - 2020-12-08
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
#include <fcntl.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "epidemic_sim.h"
#include "exchanges_between_processes.h"
#include "util.h"

struct sigaction action_sigusr1;
struct sigaction action_sigusr2;

int fifo_to_citizen_manager;

fifo_message_e message_to_citizen_manager[1];

int simulation_is_not_over = 1;

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
                    
                    //printf("[%d %d] %.3lf augmente ", col, row, city->map[col][row].contamination);///
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

    //printf("[%d %d] %.3lf à ", tile->x, tile->y, tile->contamination);///
    
    increase = generate_random_percentage_in_interval(MIN_WASTELAND_CONTAMINATION_INCREASE,
                                                      MAX_WASTELAND_CONTAMINATION_INCREASE);
    difference = other_tile_contamination - tile->contamination;

    tile->contamination += increase*difference;
    
    if (tile->contamination > MAX_CONTAMINATION) {
        tile->contamination = MAX_CONTAMINATION;
    }

    //printf("%.3lf\n", tile->contamination);///
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
    printf("Round\n"); ///
    
    // write in evolution.txt

    // A SURVEILLER
    update_wastelands_contamination();

    /* DEBUG */
    int row, col;
    printf("=====================================================\n");
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            printf("%.3lf ", city->map[col][row].contamination);
        }
        printf("\n");
    }
    
    *message_to_citizen_manager = NEXT_ROUND;
    write(fifo_to_citizen_manager, message_to_citizen_manager, sizeof(int));

    // update interface
}

void end_of_simulation()
{
    printf("End of the simulation !\n"); ///

    // Envoyer un message à citizen_manager pour lui dire que c'est fini
    *message_to_citizen_manager = END_OF_SIMULATION;
    write(fifo_to_citizen_manager, message_to_citizen_manager, sizeof(int));
    
    simulation_is_not_over = 0;
}

/* mqd_t create_mqueue(); */

int main(void)
{
    int shared_memory;

    /*mqd_t mqueue;*/
    
    srand(time(NULL));
    
    shared_memory = create_shared_memory();
    /*mqueue = create_mqueue();*/

    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    load_map(city);
    
    action_sigusr1.sa_handler = &simulation_round;
    sigaction(SIGUSR1, &action_sigusr1, NULL);

    action_sigusr2.sa_handler = &end_of_simulation;
    sigaction(SIGUSR2, &action_sigusr2, NULL);

    unlink(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL);
    if (mkfifo(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, S_IRUSR | S_IWUSR) == -1) {
        perror("Error while creating a FIFO\n");
        exit(EXIT_FAILURE);
    }
    
    fifo_to_citizen_manager = open(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL, O_WRONLY);
    
    if (fifo_to_citizen_manager == -1) {
        perror("Error while opening a FIFO\n");
        exit(EXIT_FAILURE);
    }
    
    /* TEST DEBUG MAP */
    int row, col;
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            printf("%d", city->map[col][row].type);
        }
        printf("\n");
    }
    for (row = 0; row < CITY_HEIGHT; row++) {
        for (col = 0; col < CITY_HEIGHT; col++) {
            printf("%.3lf ", city->map[col][row].contamination);
        }
        printf("\n");
    }
    
    launch_simulation();

    close(fifo_to_citizen_manager);
    unlink(FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL);
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    if (close(shared_memory) < 0) {
        perror("Error when calling close(mem)\n");
    }
    if (shm_unlink(SHARED_MEM) < 0) {
        perror("Error when calling shm_unlink()\n");
    }
    exit(EXIT_SUCCESS);
}
