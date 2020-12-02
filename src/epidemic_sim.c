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
 * @author Alain Lebret <alain.lebret@ensicaen.fr> [original author]
 * @author Jérémy Poullain <jeremy.poullain@ecole.ensicaen.fr>
 * @author Guillaume Revel <guillaume.revel@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-12-02
 */

/**
 * @file epidemic_sim.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <mqueue.h>

#include "epidemic_sim.h"

int create_shared_memory()
{
    int mem;
    
    mem = shm_open(SHARED_MEM, O_CREAT | O_RDWR, S_IRWXU);
    if (mem < 0) {
        perror("Error when calling shm_open()\n");
        exit(EXIT_FAILURE);
    }
    
    if (ftruncate(mem, sizeof(city_t)) < 0) {
        perror("Error when calling ftruncate()\n");
        exit(EXIT_FAILURE);
    }
    
    return mem;
}

void generate_city(city_t *city) {
    /* lire un fichier pour trouver les coordonnées */
    /* => les constantes HOUSES_NB, etc. deviendront inutiles */
    /*city->map[0][6] = init_tile_firestation(0, 6);
    city->map[3][3] = init_tile_hospital(3, 3);
    city->map[3][0] = init_tile_firestation(6, 0);*/
    load_map(city);
}

void load_map(city_t *city) {
    FILE *fp;
    char buffer[100];
    
    if ((fp = fopen(MAP_URL, "r")) == NULL) {
        printf("No map file\n"); /* ERREUR A CORRIGER */
        exit(-1);
    }

    while (fscanf(fp, "%[^\n]\n", buffer) != EOF) {
        if (buffer[0] == '\n' || (buffer[0] == '/' && buffer[1] == '/')) {
            continue;
        }
    }
    
    fclose(fp);
}

/* mqd_t create_mqueue(); */

int main(void)
{
    int mem;
    /*mqd_t mqueue;*/

    city_t *city;
    
    srand(time(NULL));
    mem = create_shared_memory();
    /*mqueue = create_mqueue();*/

    city = mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);
    generate_city(city);
    
    /* Début de la simulation */
    /* C'est à vous d'écrire là ... */
    
    if (munmap(city, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    if (close(mem) < 0) {
        perror("Error when calling close(mem)\n");
    }
    if (shm_unlink(SHARED_MEM) < 0) {
        perror("Error when calling shm_unlink()\n");
    }
    exit(EXIT_SUCCESS);
}
