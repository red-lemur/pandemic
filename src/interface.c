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

//city->map[col][row].type

int get_tile_color_code(int tile) {
    return tile + 1;
}

int get_state_color_code(int state) {
    switch (state) {
        case HEALTHY: return HEALTHY_CODE; break;
        case SICK: return SICK_CODE; break;
        case DEAD: return DEAD_CODE; break;
        case BURNED: return BURNED_CODE; break;
        default: return state; break;
    }
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

void set_number_of_people_in_state(int number, int state)
{
    char str[(int) log10(POPULATION_NUMBER) + 1];
    int y;



    state = get_state_color_code(state);

    //if (state == HEALTHY) mvwprintw(situation, 2, 0, "rr");

    //printf("state : %d\n", state);
    

    switch (state) {
        case HEALTHY_CODE : y = 0; break;
        case SICK_CODE : y = 2; break;
        case DEAD_CODE : y = 4; break;
        case BURNED_CODE : y = 6; break;
    }

    sprintf(str, "%d", number);

    //printf("STR : %d\n", number);

    char statestr[2];

    sprintf(statestr, "%d", state);

    /*if (state == HEALTHY)*/ //printf("STR : %s\n", str);
    

    wattron(people, COLOR_PAIR(state));
    mvwprintw(people, y, 0, str);
    wattroff(people, COLOR_PAIR(state));

}

void set_citizen_on_tile(int tile_x, int tile_y, int number, int state)
{
    int shift_right;
    int shift_bottom;

    char str[(int) log10(POPULATION_NUMBER) + 1];
    sprintf(str, "%d", number);

    if (state == SICK_CODE || state == BURNED_CODE) {
        shift_right = 1;
    } else {
        shift_right = 0;
    }

    if (state == DEAD_CODE || state == BURNED_CODE) {
        shift_bottom = 1;
    } else {
        shift_bottom = 0;
    }

    wattron(citizens, COLOR_PAIR(state));
    mvwprintw(citizens, tile_y*3 + shift_bottom, tile_x*6 + shift_right, str);
    wattroff(citizens, COLOR_PAIR(state));
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

int main(void)
{    

    printf("WASTELAND_CODE : %d\n", WASTELAND_CODE);
    //printf("HEALTHY : %d\n", HEALTHY);


    char *title = "Simulation épidémie - Jour n°";
    char *situations_title[NUMBER_OF_SITUATIONS] = { "Personnes en bonne santé",
                                                     "Personnes malades",
                                                     "Personnes décédées",
                                                     "Cadavres brûlés"};
    char *tiles[NUMBER_OF_DIF_TILES] = {"Terrain vague",
                                        "Maison",
                                        "Hôpital",
                                        "Caserne"};
    
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
        
        /* This will be replaced by sth with the text file */
        write_spaces(places, 0, 0, 6, WASTELAND_CODE);
        write_spaces(places, 0, 12, 1, FIRESTATION_CODE);
        write_spaces(places, 1, 0, 18, WASTELAND_CODE);
        write_spaces(places, 3, 6, 1, HOSPITAL_CODE);
        write_spaces(places, 3, 8, 18, WASTELAND_CODE);
        write_spaces(places, 6, 0, 1, FIRESTATION_CODE);
        write_spaces(places, 6, 2, 6, WASTELAND_CODE);
        
        for (int i = 0; i < NUMBER_OF_DIF_TILES; i++) {
            write_spaces(legend, i*2, 0, 1, HOUSE + i);
            mvwprintw(legend, i*2, 3, tiles[i]);
        }

        for (int i = 0; i < NUMBER_OF_SITUATIONS; i++) {
            mvwprintw(situation, i*2, 0, situations_title[i]);
        }
        
        set_number_of_people_in_state(POPULATION_NUMBER, HEALTHY);
        set_number_of_people_in_state(0, SICK);
        set_number_of_people_in_state(0, DEAD);
        set_number_of_people_in_state(0, BURNED);

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
