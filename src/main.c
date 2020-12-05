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

#define MAX_PID_LENGTH 7

int main(void) {
    char pid_epidemic_sim_arg[MAX_PID_LENGTH + 1];
    
    int pid_epidemic_sim;
    int pid_citizen_manager;
    int pid_timer;
    
    printf("PID du main : %d\n", getpid()); ///
    
    pid_epidemic_sim = fork();
    switch (pid_epidemic_sim) {
    case -1:
        printf("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl("./bin/epidemic_sim", "epidemic_sim", NULL);
        exit(EXIT_FAILURE);
    default:
        printf("PID de epidemic_sim : %d\n", pid_epidemic_sim); ///

        sprintf(pid_epidemic_sim_arg, "%d", pid_epidemic_sim);
        
        pid_citizen_manager = fork();
        switch (pid_citizen_manager) {
        case -1:
            printf("Error on fork()\n");
            exit(EXIT_FAILURE);
        case 0:
            execl("./bin/citizen_manager", "citizen_manager", NULL);
            exit(EXIT_FAILURE);
        default:
            printf("PID de citizen_manager : %d\n", pid_citizen_manager); ///

            pid_timer = fork();
            switch (pid_timer) {
            case -1:
                printf("Error on fork()\n");
                exit(EXIT_FAILURE);
            case 0:
                execl("./bin/timer", "timer", pid_epidemic_sim_arg, "1", NULL);
                exit(EXIT_FAILURE);
            default:
                printf("PID de timer : %d\n", pid_timer); ///
                
                exit(EXIT_SUCCESS);
            }
        }
    }
}
