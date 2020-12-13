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
 * @version 1.0.0 - 2020-12-05
 */

/**
 * @file interface.c
 *
 *
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "interface.h"

WINDOW *main_title, *titles, *places, *legend, *titles_2, *citizens, *situation, *people;

int day = 0;


int get_tile_color_code(int tile) {
    return tile + 1;
}


void write_spaces(WINDOW *win, int y, int x, int number, int color_index)
{
    number *= 2;    
    
    //char *spaces = malloc(sizeof(char)*number);

    wattron(win, COLOR_PAIR(color_index));
    for (int i = 0; i < number; i++) {
        //spaces[i] = ' ';
        mvwprintw(win, y, x + i, " ");
    }
    wattroff(win, COLOR_PAIR(color_index));
    
    //wattron(win, COLOR_PAIR(color_index));
    //mvwprintw(win, y, x, spaces);
    //wattroff(win, COLOR_PAIR(color_index));

    //free(spaces);
}

int size_of_longest_string(char **strings, int size)
{
    unsigned int longest = strlen(strings[0]);

    for (int i = 1; i < size; i++) {
        if (strlen(strings[i]) > longest) {
            longest = strlen(strings[i]);
        }
    }

    return longest;
}

/*
 * Functions to interact with the interface 
 */

void next_day()
{
    char day_str[(int) log10(NUMBER_OF_DAYS) + 1];
    
    day++;
    sprintf(day_str, "%d", day);

    wattron(main_title, COLOR_PAIR(TITLE));        
    mvwprintw(main_title, 1, (COLS / 2) + SIZE_OF_TITLE / 2, day_str);
    wattroff(main_title, COLOR_PAIR(TITLE));
}

void set_number_of_people_in_state(int number, int state_code)
{
    char str[(int) log10(POPULATION_NUMBER) + 1];
    int y;

    switch (state_code) {
        case HEALTHY_CODE : y = 0; break;
        case SICK_CODE : y = 2; break;
        case DEAD_CODE : y = 4; break;
        case BURNED_CODE : y = 6; break;
    }

    sprintf(str, "%d", number);
   

    wattron(people, COLOR_PAIR(state_code));
    mvwprintw(people, y, 0, str);
    wattroff(people, COLOR_PAIR(state_code));

}

void set_citizen_on_tile(int tile_x, int tile_y, int number, int state_code)
{
    int shift_right;
    int shift_bottom;

    char str[(int) log10(POPULATION_NUMBER) + 1];
    sprintf(str, "%d", number);

    if (state_code == SICK_CODE || state_code == BURNED_CODE) {
        shift_right = 1;
    } else {
        shift_right = 0;
    }

    if (state_code == DEAD_CODE || state_code == BURNED_CODE) {
        shift_bottom = 1;
    } else {
        shift_bottom = 0;
    }

    wattron(citizens, COLOR_PAIR(state_code));
    mvwprintw(citizens, tile_y*3 + shift_bottom, tile_x*6 + shift_right, str);
    wattroff(citizens, COLOR_PAIR(state_code));
}

void set_type_of_tile(int tile_x, int tile_y, int type)
{
    write_spaces(places, tile_y, tile_x*2, 1, get_tile_color_code(type));
}

/*
 * We need the get_citizen_on_tile defined in another file to uncomment these functions 
 * Maybe it's an object-oriented way
 */

/*void change_state_of_a_citizen(int tile, int new_state)
{
    set_citizen_on_tile(tile, get_citizen_on_tile(tile, new_state - 1) - 1, new_state - 1);
    set_citizen_on_tile(tile, get_citizen_on_tile(tile, new_state) + 1, new_state);

    set_number_of_people_in_state()
}*/

/*
 * probably useless
 */
/*void move_citizen(int starting_tile, int direction, int state)
{
    set_citizen_on_tile(starting_tile, get_citizen_on_tile(starting_tile, state) - 1, state);
    set_citizen_on_tile()
}*/

tile_t init_tile(unsigned int x, unsigned int y, int capacity,
                 building_type_e type)
{
    tile_t tile;
    
    tile.x = x;
    tile.y = y;
    tile.capacity = capacity;
    tile.type = type;
    
    return tile;
}

int main(void)
{    


    char *title = "Simulation épidémie - Jour n°";
    char *situations_title[NUMBER_OF_SITUATIONS] = { "Personnes en bonne santé",
                                                     "Personnes malades",
                                                     "Personnes décédées",
                                                     "Cadavres brûlés"};
    char *tiles[NUMBER_OF_DIF_TILES] = {"Terrain vague",
                                        "Maison",
                                        "Hôpital",
                                        "Caserne"};



    /* city example generated */
    city_t *city;
    city = malloc(sizeof(tile_t)*7*7);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            city->map[i][j] = init_tile(i, j, 16, WASTELAND); 
        }
    }

    city->map[0][6].type = FIRESTATION; 
    city->map[6][0].type = FIRESTATION;
    city->map[3][3].type = HOSPITAL;  
    city->map[1][1].type = HOUSE; 
    city->map[4][0].type = HOUSE;
    city->map[0][4].type = HOUSE;
    city->map[1][2].type = HOUSE;
    city->map[2][5].type = HOUSE;
    city->map[3][4].type = HOUSE;
    city->map[5][4].type = HOUSE;
    
    
    initscr();

    for (;;) {
        main_title = newwin(HEADER_HEIGHT, COLS, 0, 0);
        
        titles = newwin(TITLES_HEIGHT,
                        COLS,
                        HEADER_HEIGHT,
                        0);
        
        places = newwin(NUMBER_OF_DIF_TILES * 2 - 1,
                        MAP_HEIGHT * 2,
                        HEADER_HEIGHT + TITLES_HEIGHT,
                        (COLS / 2) - MAP_HEIGHT * 2 - MARGIN);
        
        legend = newwin(MAP_HEIGHT,
                        (COLS / 2) - MARGIN,
                        HEADER_HEIGHT + TITLES_HEIGHT,
                        (COLS / 2) + MARGIN);

        titles_2 = newwin(TITLES_HEIGHT,
                          COLS,
                          HEADER_HEIGHT + TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN,
                          0);
        
        citizens = newwin(MAP_HEIGHT*2 + (MAP_HEIGHT-1),
                          (MAP_HEIGHT*2 + (MAP_HEIGHT-1)) * 2,
                          HEADER_HEIGHT + 2*TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN,
                          (COLS / 2) - ((MAP_HEIGHT * 2 + (MAP_HEIGHT - 1)) * 2) - MARGIN);
        
        situation = newwin(2 * NUMBER_OF_SITUATIONS - 1,
                           size_of_longest_string(situations_title, NUMBER_OF_SITUATIONS),
                           HEADER_HEIGHT + 2*TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN,
                           (COLS / 2) + MARGIN);
        
        people = newwin(2 * NUMBER_OF_SITUATIONS - 1,
                        (int)log10(POPULATION_NUMBER) + 1,
                        HEADER_HEIGHT + 2 * TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN,
                        (COLS / 2) + MARGIN
                        + size_of_longest_string(situations_title, NUMBER_OF_SITUATIONS)
                        + SPACE_SITUATION);
        
        start_color();
        init_pair(TITLE, COLOR_BLACK, COLOR_WHITE);
        
        init_pair(HOUSE_CODE, COLOR_BLACK, COLOR_GREEN);
        init_pair(HOSPITAL_CODE, COLOR_BLACK, COLOR_BLUE);
        init_pair(FIRESTATION_CODE, COLOR_BLACK, COLOR_RED);
        init_pair(WASTELAND_CODE, COLOR_BLACK, COLOR_YELLOW);

        init_pair(HEALTHY_CODE, COLOR_GREEN, COLOR_BLACK);
        init_pair(SICK_CODE, COLOR_YELLOW, COLOR_BLACK);
        init_pair(DEAD_CODE, COLOR_BLUE, COLOR_BLACK);
        init_pair(BURNED_CODE, COLOR_RED, COLOR_BLACK);

        refresh();
               
        box(main_title,0,0);        
        wattron(main_title, COLOR_PAIR(TITLE));        
        mvwprintw(main_title, 1, (COLS / 2) - (strlen(title) / 2), title);
        wattroff(main_title, COLOR_PAIR(TITLE));
        next_day();

        wattron(titles, A_UNDERLINE);
        mvwprintw(titles, 1, (COLS/2) - MAP_HEIGHT*2 - MARGIN, "Cartes des lieux");
        mvwprintw(titles, 1, (COLS/2) + MARGIN, "Légende");
        wattroff(titles, A_UNDERLINE);

        wattron(titles_2, A_UNDERLINE);
        mvwprintw(titles_2, 1, (COLS/2) - ((MAP_HEIGHT*2 + (MAP_HEIGHT-1)) *2) - MARGIN, "Cartes des citoyens");
        mvwprintw(titles_2, 1, (COLS/2) + MARGIN, "Evolution de l'épidémie");
        wattroff(titles_2, A_UNDERLINE);
        

        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_HEIGHT; j++) {
                set_type_of_tile(i, j, city->map[i][j].type);
            }            
        }
        
        for (int i = 0; i < NUMBER_OF_DIF_TILES; i++) {
            write_spaces(legend, i*2, 0, 1, HOUSE + i);
            mvwprintw(legend, i*2, 3, tiles[i]);
        }

        for (int i = 0; i < NUMBER_OF_SITUATIONS; i++) {
            mvwprintw(situation, i*2, 0, situations_title[i]);
        }
        
        set_number_of_people_in_state(POPULATION_NUMBER, HEALTHY_CODE);
        set_number_of_people_in_state(0, SICK_CODE);
        set_number_of_people_in_state(0, DEAD_CODE);
        set_number_of_people_in_state(0, BURNED_CODE);

        for (int s = HEALTHY_CODE; s <= BURNED_CODE; s++) {
            for (int i=0; i < MAP_HEIGHT; i++) {
                for(int j=0; j< MAP_HEIGHT; j++) {
                    set_citizen_on_tile(i, j, 0, s);
                }        
            } 
        }

        
        wrefresh(titles);
        wrefresh(places);
        wrefresh(legend);
        wrefresh(titles_2);
        wrefresh(citizens);
        wrefresh(situation);
        wrefresh(people);
        wrefresh(main_title);
   
        if (getch() != 410) {
            break;
        }
    }

    delwin(people);
    delwin(situation);
    delwin(citizens);
    delwin(titles_2);
    delwin(legend);
    delwin(places);
    delwin(titles);
    delwin(main_title);

    endwin();

    /*
    free(people);
    free(situation);
    free(citizens);
    free(titles_2);
    free(legend);
    free(places);
    free(titles);
    free(main_title);*/
 
    return 0;
}
