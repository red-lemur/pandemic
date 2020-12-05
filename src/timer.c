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
 * @version 1.0.0 - 2020-12-05
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

void tick()
{    
    if (tour_nb < TOTAL_TOUR_NB) {
        tour_nb++;
        alarm(tour_duration);
    }
    else {
        kill(epidemic_sim_pid, SIGUSR1);
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s pid tour_duration\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    epidemic_sim_pid = atoi(argv[1]);
    tour_duration = atoi(argv[2]);
    
    if (tour_duration < 1 || tour_duration > 5) {
        printf("Duration must be between 1 and 5 seconds!\n");
        exit(EXIT_FAILURE);
    }    
    
    action.sa_handler = &tick;
    sigaction(SIGALRM, &action, NULL);
    alarm(tour_duration);
    
    for(;;);
}
