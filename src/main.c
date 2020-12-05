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
 * @file main.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int pid;

    printf("PID du main : %d\n", getpid()); ///
    
    pid = fork();
    switch (pid) {
    case -1:
        printf("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl("./bin/epidemic_sim", "epidemic_sim", NULL);
        exit(EXIT_FAILURE);
    default:
        printf("PID de epidemic_sim : %d\n", pid); ///
        
        pid = fork();
        switch (pid) {
        case -1:
            printf("Error on fork()\n");
            exit(EXIT_FAILURE);
        case 0:
            execl("./bin/timer", "timer", "1", NULL);
            exit(EXIT_FAILURE);
        default:
            printf("PID de timer : %d\n", pid); ///
            
            exit(EXIT_SUCCESS);
        }
    }
}
