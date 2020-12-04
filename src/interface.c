#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MARGIN 10
#define VERTICAL_MARGIN 2
#define HEADER_HEIGHT 3
#define TITLES_HEIGHT 3
#define MAP_HEIGHT 7
#define SPACE_SITUATION 3
#define BOTTOM_MARGIN 5
#define POPULATION_NUMBER 25
#define NUMBER_OF_DIF_TILES 4
#define NUMBER_OF_SITUATIONS 4
#define SIZE_OF_TITLE 32

#define NUMBER_OF_DAYS 10

#define HOUSE 2
#define HOSPITAL 3
#define FIRESTATION 4
#define WASTELAND 5

#define HEALTHY 6
#define SICK 7
#define DEAD 8
#define BURNT 9

#define UP_LEFT 1
#define UP 2
#define UP_RIGHT 3
#define RIGHT 4
#define BOTTOM_RIGHT 5
#define BOTTOM 6
#define BOTTOM_LEFT 7
#define LEFT 8


WINDOW *main_title, *titles, *places, *legend, *titles_2, *citizens, *situation, *people, *instruction;

int day = 0;

void write_spaces(WINDOW *win, int y, int x, int number, int color_index) {
    number *= 2;    
    
    char *spaces = malloc(sizeof(char)*number);

    for (int i = 0; i < number; i++) {
        spaces[i] = ' ';
    }

    wattron(win, COLOR_PAIR(color_index));
    mvwprintw(win, y, x, spaces);
    wattroff(win, COLOR_PAIR(color_index));
}


int size_of_longest_string(char **strings, int size) {
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

void next_day() {
    day++;
    char day_str[(int) log10(NUMBER_OF_DAYS) + 1];
    sprintf(day_str, "%d", day);

    wattron(main_title, COLOR_PAIR(1));        
    mvwprintw(main_title, 1, (COLS / 2) + SIZE_OF_TITLE / 2, day_str);
    wattroff(main_title, COLOR_PAIR(1));
}


void set_number_of_people_in_state(int number, int state) {
    char str[(int) log10(POPULATION_NUMBER) + 1];
    int y;

    switch (state) {
        case HEALTHY : y = 0; break;
        case SICK : y = 2; break;
        case DEAD : y = 4; break;
        case BURNT : y = 6; break;
    }

    sprintf(str, "%d", number);

    wattron(people, COLOR_PAIR(state));
    mvwprintw(people, y, 0, str);
    wattroff(people, COLOR_PAIR(state));
}


void set_citizen_on_tile(int tile_x, int tile_y, int number, int state) {
    int shift_right;
    int shift_bottom;

    char str[(int) log10(POPULATION_NUMBER) + 1];
    sprintf(str, "%d", number);

    if (state == SICK || state == BURNT) {
        shift_right = 1;
    }
    else {
        shift_right = 0;
    }

    if (state == DEAD || state == BURNT) {
        shift_bottom = 1;
    }
    else {
        shift_bottom = 0;
    }

    wattron(citizens, COLOR_PAIR(state));
    mvwprintw(citizens, tile_y*3 + shift_bottom, tile_x*6 + shift_right, str);
    wattroff(citizens, COLOR_PAIR(state));
}

void set_type_of_tile(int tile_x, int tile_y, int type) {
    write_spaces(places, tile_y, tile_x*2, 1, type);
}


/*
 * We need the get_citizen_on_tile defined in another file to uncomment these functions 
 * Maybe it's an object-oriented way
 */

/*void change_state_of_a_citizen(int tile, int new_state) {
    set_citizen_on_tile(tile, get_citizen_on_tile(tile, new_state - 1) - 1, new_state - 1);
    set_citizen_on_tile(tile, get_citizen_on_tile(tile, new_state) + 1, new_state);

    set_number_of_people_in_state()
}*/

/*
 * probably useless
 */
/*void move_citizen(int starting_tile, int direction, int state) {
    set_citizen_on_tile(starting_tile, get_citizen_on_tile(starting_tile, state) - 1, state);
    set_citizen_on_tile()
}*/



int main(void)
{	
    
    char *title = "Simulation épidémie - Jour n°";
    char *instr_msg = "Appuyez sur une touche pour passer au jour suivant";
    char *situations_title[NUMBER_OF_SITUATIONS] = {"Personnes en bonne santé", "Personnes malades", "Personnes décédées", "Cadavres brûlés"};
    char *tiles[NUMBER_OF_DIF_TILES] = {"Maison", "Hôpital", "Caserne", "Terrain vague"};
    
    initscr();

    while(1) {

        main_title = newwin(HEADER_HEIGHT, COLS, 0, 0); 
        titles = newwin(TITLES_HEIGHT, COLS, HEADER_HEIGHT, 0);
        places = newwin(NUMBER_OF_DIF_TILES*2 - 1, MAP_HEIGHT*2, HEADER_HEIGHT + TITLES_HEIGHT, (COLS/2) - MAP_HEIGHT*2 - MARGIN);
        legend = newwin(MAP_HEIGHT, (COLS/2) - MARGIN, HEADER_HEIGHT + TITLES_HEIGHT, (COLS/2) + MARGIN);

        titles_2 = newwin(TITLES_HEIGHT, COLS, HEADER_HEIGHT + TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN, 0);
        citizens = newwin(MAP_HEIGHT*2 + (MAP_HEIGHT-1), (MAP_HEIGHT*2 + (MAP_HEIGHT-1)) *2, HEADER_HEIGHT + 2*TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN, (COLS/2) - ((MAP_HEIGHT*2 + (MAP_HEIGHT-1)) *2) - MARGIN);
        situation = newwin(2*NUMBER_OF_SITUATIONS - 1, size_of_longest_string(situations_title, NUMBER_OF_SITUATIONS), HEADER_HEIGHT + 2*TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN, (COLS/2) + MARGIN);
        people = newwin(2*NUMBER_OF_SITUATIONS - 1, (int) log10(POPULATION_NUMBER) + 1, HEADER_HEIGHT + 2*TITLES_HEIGHT + MAP_HEIGHT + VERTICAL_MARGIN, (COLS/2) + MARGIN + size_of_longest_string(situations_title, NUMBER_OF_SITUATIONS) + SPACE_SITUATION);
        instruction = newwin(TITLES_HEIGHT, strlen(instr_msg) + 1, LINES - BOTTOM_MARGIN, COLS - strlen(instr_msg) - BOTTOM_MARGIN);
        

        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);

        init_pair(2, COLOR_BLACK, COLOR_GREEN);
        init_pair(3, COLOR_BLACK, COLOR_BLUE);
        init_pair(4, COLOR_BLACK, COLOR_RED);
        init_pair(5, COLOR_BLACK, COLOR_YELLOW);

        init_pair(6, COLOR_GREEN, COLOR_BLACK);
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);
        init_pair(8, COLOR_BLUE, COLOR_BLACK);
        init_pair(9, COLOR_RED, COLOR_BLACK);



        refresh();
        
        
        box(main_title,0,0);        
        wattron(main_title, COLOR_PAIR(1));        
        mvwprintw(main_title, 1, (COLS / 2) - (strlen(title) / 2), title);
        wattroff(main_title, COLOR_PAIR(1));
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
        write_spaces(places, 0, 0, 6, WASTELAND);
        write_spaces(places, 0, 12, 1, FIRESTATION);
        write_spaces(places, 1, 0, 18, WASTELAND);
        write_spaces(places, 3, 6, 1, HOSPITAL);
        write_spaces(places, 3, 8, 18, WASTELAND);
        write_spaces(places, 6, 0, 1, FIRESTATION);
        write_spaces(places, 6, 2, 6, WASTELAND);
        



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
        set_number_of_people_in_state(0, BURNT);


        for (int s = HEALTHY; s <= BURNT; s++) {
            for (int i=0; i < MAP_HEIGHT; i++) {
                for(int j=0; j< MAP_HEIGHT; j++) {
                    set_citizen_on_tile(i, j, 0, s);
                }        
            } 
        }


        wattron(instruction, COLOR_PAIR(1));
        mvwprintw(instruction, 0, 0, instr_msg);
        wattroff(instruction, COLOR_PAIR(1));



        wrefresh(main_title);
        wrefresh(titles);
        wrefresh(places);
        wrefresh(legend);
        wrefresh(titles_2);
        wrefresh(citizens);
        wrefresh(situation);
        wrefresh(people);
        wrefresh(instruction);
   
        if (getch() != 410) {
            break;
        }
    }

    delwin(main_title);
    delwin(titles);
    delwin(places);
    delwin(legend);
    delwin(titles_2);
    delwin(citizens);
    delwin(situation);
    delwin(people);
    delwin(instruction);  

    endwin();
 
    free(main_title);
    free(titles);
    free(places);
    free(legend);
    free(titles_2);
    free(citizens);
    free(situation);
    free(people);
    free(instruction);
 
    return 0;
}


