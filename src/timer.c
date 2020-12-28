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
 * @author Jérémy Poullain <jeremy.poullain@ecole.ensicaen.fr>
 * @author Guillaume Revel <guillaume.revel@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-12-28
 */

/**
 * @file timer.c
 *
 * A clock that emit an alarm signal every tour of a given duration.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "timer.h"

struct sigaction action;

pid_t epidemic_sim_pid;
int game_round_duration;
int game_round_nb = 0;

void tick()
{    
        if (game_round_nb < TOTAL_TOUR_NB) {
                game_round_nb++;
                kill(epidemic_sim_pid, SIGUSR1);
                alarm(game_round_duration);
        }
        else {
                kill(epidemic_sim_pid, SIGUSR2);
                exit(EXIT_SUCCESS);
        }
}

int main(int argc, char* argv[])
{
        if (argc != 3) {
                printf("Usage: %s pid game_round_duration\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        
        epidemic_sim_pid = atoi(argv[1]);
        game_round_duration = atoi(argv[2]);
        
        if (game_round_duration < 1 || game_round_duration > 5) {
                perror("Duration must be between 1 and 5 seconds!\n");
                exit(EXIT_FAILURE);
        }    
        
        action.sa_handler = &tick;
        sigaction(SIGALRM, &action, NULL);
        alarm(game_round_duration);
        
        for(;;);
}
