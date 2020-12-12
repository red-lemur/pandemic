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
 * @file exchanges_between_processes.h
 *
 * Constants to manage the exchanges between processes.
 */

#ifndef EXCHANGES_BETWEEN_PROCESSES_H
#define EXCHANGES_BETWEEN_PROCESSES_H

#define SHARED_MEM "/simulation"
#define FIFO_EPIDEMIC_SIM_TO_CITIZEN_MANAGER_URL "/tmp/epidemic_sim_to_citizen_manager"

typedef enum { NEXT_ROUND, END_OF_SIMULATION } fifo_message_e;

#endif
