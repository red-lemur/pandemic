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
 * @version 1.0.0 - 2020-12-12
 */

/**
 * @file main.c
 *
 * Main program that launches all the processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file_paths.h"

#define MAX_PID_LENGTH 7

int main(void)
{
    char pid_epidemic_sim_arg[MAX_PID_LENGTH + 1];
    
    int pid_epidemic_sim;
    int pid_citizen_manager;
    int pid_timer;
    
    //printf("PID du main : %d\n", getpid()); ///
    
    pid_epidemic_sim = fork();

    switch (pid_epidemic_sim) {
    case -1:
        perror("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl(EPIDEMIC_SIM_URL, EPIDEMIC_SIM_NAME, NULL);
        exit(EXIT_FAILURE);
    }
    
    //printf("PID de epidemic_sim : %d\n", pid_epidemic_sim); ///
    
    sprintf(pid_epidemic_sim_arg, "%d", pid_epidemic_sim);
    
    pid_citizen_manager = fork();

    switch (pid_citizen_manager) {
    case -1:
        perror("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl(CITIZEN_MANAGER_URL, CITIZEN_MANAGER_NAME, NULL);
        exit(EXIT_FAILURE);
    }
    
    //printf("PID de citizen_manager : %d\n", pid_citizen_manager); ///
    
    pid_timer = fork();

    switch (pid_timer) {
    case -1:
        perror("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl(TIMER_URL, TIMER_NAME, pid_epidemic_sim_arg, TIMER_ARG, NULL);
        exit(EXIT_FAILURE);
    }
    
    //printf("PID de timer : %d\n", pid_timer); ///
    
    exit(EXIT_SUCCESS);
}
