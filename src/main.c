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
 * @version 1.0.0 - 2020-11-18
 */

/**
 * @file main.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /* close */
#include <sys/mman.h> /* shm_open and shm_unlink */
#include <sys/stat.h> /* constantes des modes */
#include <fcntl.h> /* constantes O_XXX */

#define SHM_SIZE 100

int main() {
    int* ptr;
    int fd, i;
    pid_t pid;
    
    srand(time(NULL));

    fd = shm_open("/simulation", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SHM_SIZE);
    ptr = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid = fork();

    if (pid > 0) {
        for (i = 0; i < SHM_SIZE; i++) {
            /*ptr[i] = rand()%SHM_SIZE;*/
            ptr[i] = i;
            printf("%d ", ptr[i]);
        }
        printf("\n");
    } else {
        for (i = 0; i < SHM_SIZE; i++) {
            printf("%d ", ptr[i]);
        }
        printf("\n");
    }
    
    munmap(ptr, SHM_SIZE);
    close(fd);

    if (pid > 0) {
        shm_unlink("/simulation");
    }
    
    return EXIT_SUCCESS;
}
