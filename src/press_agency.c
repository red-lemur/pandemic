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
 * @file press_agency.c
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "press_agency.h"
#include "exchanges_between_processes.h"

mqd_t create_mqueue()
{
    mqd_t mq;
    
    mq = mq_open(MQUEUE, O_RDONLY | O_CREAT, 0600, NULL);
    if (mq == (mqd_t) -1) {
        perror("Error when calling mq_open()\n");
        exit(EXIT_FAILURE);
    }
    
    return mq;
}

int main(void)
{
    mqd_t mq;

    mq = create_mqueue();

    printf("+------------------------------------------------------------------------------+\n");
    printf("|                                BREAKING  NEWS                                |\n");
    printf("+------------------------------------------------------------------------------+\n");
    
    if (mq_close(mq) < 0) {
        perror("Error when calling mq_close(mq)\n");
    }
    
    if (mq_unlink(MQUEUE) < 0) {
        perror("Error when calling mq_unlink()\n");
    }

    for (;;);
    
    exit(EXIT_SUCCESS);
}
