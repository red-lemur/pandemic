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
 * @file file_paths.h
 *
 * Constants defining the paths and args of program files.
 */

#ifndef FILE_PATHS_H
#define FILE_PATHS_H

#define EPIDEMIC_SIM_URL "./bin/epidemic_sim"
#define EPIDEMIC_SIM_NAME "epidemic_sim"

#define PRESS_AGENCY_COMMAND "gnome-terminal --geometry 80x10+1000+600 -- ./bin/press_agency"

#define CITIZEN_MANAGER_URL "./bin/citizen_manager"
#define CITIZEN_MANAGER_NAME "citizen_manager"

#define TIMER_URL "./bin/timer"
#define TIMER_NAME "timer"
#define TIMER_ARG "1"

#define EVOLUTION_URL "./etc/evolution.txt"
#define COMMANDS_URL "./etc/commands.gp"
#define SOS_URL "./etc/sos"

#endif
