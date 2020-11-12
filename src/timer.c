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
 * @version 1.0.0 - 2020-11-07
 */

/**
 * @file timer.c
 *
 * A clock that emit an alarm signal every tour of a given duration.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

struct sigaction action;
int duration = 0;

/**
 * @brief Emit a signal which call this function again.
 */
void tick() {
  printf("renard");
    alarm(duration);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s tour_duration\n", argv[0]);
        exit(-1);
    }

    duration = atoi(argv[1]);

    if (duration < 1 || duration > 5) {
        printf("Duration must be between 1 and 5 seconds!\n");
        exit(-1);
    }
    
    action.sa_handler = &tick;
    sigaction(SIGALRM, &action, NULL);
    alarm(duration);
    
    for(;;);
}
