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
 * @version 1.0.0 - 2020-11-30
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

#include "timer.h"

struct sigaction action;

void tick()
{
    alarm(TOUR_DURATION);
}

int main(void)
{
    action.sa_handler = &tick;
    sigaction(SIGALRM, &action, NULL);
    alarm(TOUR_DURATION);
    
    for(;;);
}
