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
 * @version 1.0.0 - 2020-12-22
 */

/**
 * @file main.c
 *
 * Main program that launches all the processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_paths.h"
#include "exchanges_between_processes.h"

#define MAX_PID_LENGTH 7

void make_sos_file(pid_t epidemic_sim_pid, pid_t citizen_manager_pid, pid_t timer_pid)
{
    FILE* file = NULL;
    int fd;

    fd = open(SOS_URL, O_RDWR | O_CREAT, 0777);
    file = fdopen(fd, "w");
    if (file != NULL) {
        fprintf(file, "#!/bin/sh\n");
	fprintf(file, "kill -9 %d\n", epidemic_sim_pid);
        fprintf(file, "kill -9 %d\n", citizen_manager_pid);
        fprintf(file, "kill -9 %d\n", timer_pid);
        fprintf(file, "rm %s\n", FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL);
        fprintf(file, "rm %s\n", FIFO_PRESS_AGENCY_TO_EPIDEMIC_SIM_URL);
        fclose(file);
    }
}

int main(void)
{
    char epidemic_sim_pid_arg[MAX_PID_LENGTH + 1];
    
    pid_t epidemic_sim_pid;
    pid_t citizen_manager_pid;
    pid_t timer_pid;
    
    epidemic_sim_pid = fork();

    switch (epidemic_sim_pid) {
    case -1:
        perror("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl(EPIDEMIC_SIM_URL, EPIDEMIC_SIM_NAME, NULL);
        exit(EXIT_FAILURE);
    }
    
    sprintf(epidemic_sim_pid_arg, "%d", epidemic_sim_pid);
    
    system(PRESS_AGENCY_COMMAND);
    
    citizen_manager_pid = fork();
    
    switch (citizen_manager_pid) {
    case -1:
        perror("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl(CITIZEN_MANAGER_URL, CITIZEN_MANAGER_NAME, NULL);
        exit(EXIT_FAILURE);
    }
    
    timer_pid = fork();

    switch (timer_pid) {
    case -1:
        perror("Error on fork()\n");
        exit(EXIT_FAILURE);
    case 0:
        execl(TIMER_URL, TIMER_NAME, epidemic_sim_pid_arg, TIMER_ARG, NULL);
        exit(EXIT_FAILURE);
    }
    
    make_sos_file(epidemic_sim_pid, citizen_manager_pid, timer_pid);
    
    exit(EXIT_SUCCESS);
}
