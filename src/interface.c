#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MARGIN 10
int main(void)
{	

    WINDOW *maintitle, *titles, *places, *legend, *titles2, *citizens, *situation, *instruction;
    char *title = "Simulation épidémie - Jour n°1";
    char *legendtitle = "Légende";
    
    initscr();
    maintitle= newwin(3, COLS, 0, 0); 
    titles= newwin(3, COLS, 3, 0);
    places= newwin(7, 14, 6, (COLS/2) - 14 - MARGIN);
    legend= newwin(7, (COLS/2) - MARGIN, 6, (COLS/2) + MARGIN);

    titles2= newwin(7, (COLS/2) - MARGIN, 6, (COLS/2) + MARGIN);
    citizens= newwin(7, (COLS/2) - MARGIN, 6, (COLS/2) + MARGIN);
    situation= newwin(7, (COLS/2) - MARGIN, 6, (COLS/2) + MARGIN);
    instruction= newwin(7, (COLS/2) - MARGIN, 6, (COLS/2) + MARGIN);
    

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    

    refresh();

    box(maintitle,0,0);


    wattron(maintitle, COLOR_PAIR(1));
    mvwprintw(maintitle, 1, (COLS / 2) - (strlen(title) / 2), title);
    wattroff(maintitle, COLOR_PAIR(1));


    wattron(titles, A_UNDERLINE);
    mvwprintw(titles, 1, (COLS/2) - 14 - MARGIN, "Cartes des lieux");
    mvwprintw(titles, 1, (COLS/2) + MARGIN, legendtitle);
    wattroff(titles, A_UNDERLINE);


    wattron(places, COLOR_PAIR(2));
    mvwprintw(places, 0, 0, "            ");
    wattroff(places, COLOR_PAIR(2));
    wattron(places, COLOR_PAIR(3));
    mvwprintw(places, 0, 12, "  ");
    wattroff(places, COLOR_PAIR(3));
    wattron(places, COLOR_PAIR(2));
    mvwprintw(places, 1, 0, "                                   ");
    wattroff(places, COLOR_PAIR(2));
    wattron(places, COLOR_PAIR(4));
    mvwprintw(places, 3, 6, "  ");
    wattroff(places, COLOR_PAIR(4));
    wattron(places, COLOR_PAIR(2));
    mvwprintw(places, 3, 8, "                                  ");
    wattroff(places, COLOR_PAIR(2));
    wattron(places, COLOR_PAIR(3));
    mvwprintw(places, 6, 0, "  ");
    wattroff(places, COLOR_PAIR(3));
    wattron(places, COLOR_PAIR(2));
    mvwprintw(places, 6, 2, "            ");
    wattroff(places, COLOR_PAIR(2));
    
    

    wattron(legend, COLOR_PAIR(5));
    mvwprintw(legend, 0, 0, "  ");
    wattroff(legend, COLOR_PAIR(5));
    mvwprintw(legend, 0, 2, " Maison");
    wattron(legend, COLOR_PAIR(4));
    mvwprintw(legend, 2, 0, "  ");
    wattroff(legend, COLOR_PAIR(4));
    mvwprintw(legend, 2, 2, " Hôpital");
    wattron(legend, COLOR_PAIR(3));
    mvwprintw(legend, 4, 0, "  ");
    wattroff(legend, COLOR_PAIR(3));
    mvwprintw(legend, 4, 2, " Caserne");
    wattron(legend, COLOR_PAIR(2));
    mvwprintw(legend, 6, 0, "  ");
    wattroff(legend, COLOR_PAIR(2));
    mvwprintw(legend, 6, 2, " Terrain vague");




    wrefresh(maintitle);
    wrefresh(titles);
    wrefresh(places);
    wrefresh(legend);
    wrefresh(titles2);
    wrefresh(citizens);
    wrefresh(situation);
    wrefresh(instruction);

    getch();

    delwin(maintitle);
    delwin(titles);
    delwin(places);
    delwin(legend);
    delwin(titles2);
    delwin(citizens);
    delwin(situation);
    delwin(instruction);

    endwin();
 
    free(maintitle);
    free(titles);
    free(places);
    free(legend);
    free(titles2);
    free(citizens);
    free(situation);
    free(instruction);

 
    return 0;
}
