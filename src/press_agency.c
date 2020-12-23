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
 * @version 1.0.0 - 2020-12-23
 */

/**
 * @file press_agency.c
 * 
 * Receive news from the journalists who circulate in the city and post them continuously.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <math.h>

#include "press_agency.h"
#include "city.h"
#include "citizen_manager.h"
#include "exchanges_between_processes.h"

mqd_t mq;
struct mq_attr attr;

struct sigaction action;

int fifo_to_epidemic_sim;
fifo_message_e message_to_epidemic_sim[1];

char *buffer = NULL;
unsigned int *priority = NULL;

char journalists_name[JOURNALISTS_NB][CITIZEN_NAME_MAX_LENGTH];
char journalist_name[CITIZEN_NAME_MAX_LENGTH];

double journalists_contamination[JOURNALISTS_NB];
double journalist_contamination;

double city_mean_contamination;

unsigned int journalists_id[JOURNALISTS_NB];
unsigned int journalist_id;

int journalists_dead[JOURNALISTS_NB];

int citizens_sick_number;
int deads_number;

mqd_t create_mqueue()
{
    mqd_t mq;
    
    mq = mq_open(MQUEUE, O_RDONLY | O_CREAT | O_NONBLOCK, 0600, NULL);
    if (mq == (mqd_t) -1) {
        perror("Error when calling mq_open()\n");
        exit(EXIT_FAILURE);
    }
    
    if (mq_getattr(mq, &attr) != 0) {
        perror("Error when calling mq_getattr()\n");
        exit(EXIT_FAILURE);
    }
    
    return mq;
}

void allocate_receive_parameters()
{
    buffer = malloc(attr.mq_msgsize * sizeof(char));
    if (buffer == NULL) {
        perror("Error when calling malloc()\n");
        exit(EXIT_FAILURE);
    }
    
    priority = malloc(sizeof(unsigned int));
    if (priority == NULL) {
        perror("Error when calling malloc()\n");
        exit(EXIT_FAILURE);
    }
}

void receive_news()
{    
    int i;

    for (i = 0; i < JOURNALISTS_NB; i++) {
        journalists_id[i] = 0;
        journalists_contamination[i] = 0;
        journalists_dead[i] = 0;
    }
    
    citizens_sick_number = -1;
    city_mean_contamination = -1;
    deads_number = -1;
    for (;;) {        
        if (mq_receive(mq, buffer, attr.mq_msgsize, priority) > 0) {
            if (all_journalists_are_dead()) {
                press_agency_zombie();
                continue;
            } else {
                store_news(&priority, buffer);            
                print_news();
            }
        }
    }
}

void store_news()
{
    int i;
    
    switch (*priority) {
    case CITIZENS_CONTAMINATION_PRIORITY:
        citizens_sick_number = atoi(buffer);
        break;
    case CITY_CONTAMINATION_PRIORITY:
        city_mean_contamination = strtod(buffer, NULL);
        break;
    case DEADS_NUMBER_PRIORITY:
        deads_number = atoi(buffer);
        break;
    case PERSONNAL_CONTAMINATION_PRIORITY:
        sscanf(buffer, "%ud %lf %s", &journalist_id, &journalist_contamination,
               journalist_name);
        
        for (i = 0; i < JOURNALISTS_NB; i++) {
            if (!journalists_id[i] || journalists_id[i] == journalist_id) {
                if (!journalists_id[i]) {
                    journalists_id[i] = journalist_id;
                    strcpy(journalists_name[i], journalist_name);
                }
                journalists_contamination[i] = journalist_contamination;
                break;
            }
        }
        break;
    case DEAD_PRIORITY:
        sscanf(buffer, "%ud", &journalist_id);
        
        for (i = 0; i < JOURNALISTS_NB; i++) {
            if (!journalists_id[i] || journalists_id[i] == journalist_id) {
                if (!journalists_id[i]) {
                    journalists_id[i] = journalist_id;
                }
                journalists_dead[i] = 1;
                break;
            }
        }
    }
}

int all_journalists_are_dead()
{
    int i;
    
    for (i = 0; i < JOURNALISTS_NB; i++) {
        if (!journalists_dead[i]) {
            return 0;
        }
    }

    return 1;
}

void print_header()
{
    printf("+------------------------------------------------------------------------------+\n");
    printf("|                                BREAKING  NEWS                                |\n");
    printf("+------------------------------------------------------------------------------+\n");
}

void print_news()
{
    int i;
    
    system("clear");
    print_header();
    
    if (citizens_sick_number != -1) {
        printf("Nombre total de citoyens malades : %d\n",
               citizens_sick_number - (int) round(citizens_sick_number
                                                  * CONTAMINATION_MINORATION));
    }
    
    if (city_mean_contamination != -1) {
        printf("Niveau moyen de contamination de la ville : %.3lf\n",
               city_mean_contamination - city_mean_contamination * CONTAMINATION_MINORATION);
    }
    
    if (deads_number != -1) {
        printf("Nombre total de morts : %d\n",
               deads_number - (int) round(deads_number * DEADS_MINORATION));
    }
    
    for (i = 0; i < JOURNALISTS_NB; i++) {
        if (journalists_id[i] && !journalists_dead[i]
            && journalists_contamination[i] >= MIN_JOURNALIST_CONTAMINATION_TO_DISPLAY) {
            printf("Taux de contamination du journaliste %s : %.3lf\n",
                   journalists_name[i], journalists_contamination[i]);
        }
    }
}

void press_agency_zombie()
{
    system("clear");
    printf("                                     uuuuuuu\n");
    printf("                                 uu$$$$$$$$$$$uu\n");
    printf("                              uu$$$$$$$$$$$$$$$$$uu\n");
    printf("                             u$$$$$$$$$$$$$$$$$$$$$u\n");
    printf("                            u$$$$$$$$$$$$$$$$$$$$$$$u\n");
    printf("                           u$$$$$$$$$$$$$$$$$$$$$$$$$u\n");
    printf("                           u$$$$$$$$$$$$$$$$$$$$$$$$$u\n");
    printf("                           u$$$$$$\"   \"$$$\"   \"$$$$$$u\n");
    printf("                           \"$$$$\"      u$u       $$$$\"\n");
    printf("                            $$$u       u$u       u$$$\n");
    printf("                            $$$u      u$$$u      u$$$\n");
    printf("                             \"$$$$uu$$$   $$$uu$$$$\"\n");
    printf("                              \"$$$$$$$\"   \"$$$$$$$\"\n");
    printf("                                u$$$$$$$u$$$$$$$u\n");
    printf("                                 u$\"$\"$\"$\"$\"$\"$u\n");
    printf("                                 $$u$ $ $ $ $u$$\n");
    printf("                                  $$$$$u$u$u$$$\n");
    printf("                                   \"$$$$$$$$$\"\n");
    printf("                                      \"\"\"\"\"\n");
}

void end_press_agency()
{
    free(priority);
    free(buffer);
    
    if (mq_close(mq) < 0) {
        perror("Error when calling mq_close(mq)\n");
    }
    
    if (mq_unlink(MQUEUE) < 0) {
        perror("Error when calling mq_unlink()\n");
    }
    
    exit(EXIT_SUCCESS);
}

int main(void)
{
    pid_t pid;
    
    action.sa_handler = &end_press_agency;
    sigaction(SIGUSR1, &action, NULL);
    
    mq = create_mqueue();
    
    do {
        fifo_to_epidemic_sim = open(FIFO_PRESS_AGENCY_TO_EPIDEMIC_SIM_URL, O_WRONLY);
    } while (fifo_to_epidemic_sim == -1);
    
    pid = getpid();
    write(fifo_to_epidemic_sim, &pid, sizeof(pid_t));
    
    allocate_receive_parameters();
    receive_news();
}
