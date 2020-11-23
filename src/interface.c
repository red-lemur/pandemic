#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MARGIN 10

int main(void)
{	

    WINDOW *maintitle, *titles, *places, *legend, *titles2, *citizens, *situation, *people, *instruction;
    char *title = "Simulation épidémie - Jour n°1";
    char *instrMsg = "Appuyez sur une touche pour passer au jour suivant";
    char *longestSituation = "Personnes en bonne santé";
    
    initscr();
    maintitle= newwin(3, COLS, 0, 0); 
    titles= newwin(3, COLS, 3, 0);
    places= newwin(7, 14, 6, (COLS/2) - 14 - MARGIN);
    legend= newwin(7, (COLS/2) - MARGIN, 6, (COLS/2) + MARGIN);

    titles2= newwin(3, COLS, 15, 0);
    citizens= newwin(20, 40, 18, (COLS/2) - 40 - MARGIN);
    situation= newwin(7, strlen(longestSituation), 18, (COLS/2) + MARGIN);
    people= newwin(7, 2, 18, (COLS/2) + MARGIN + strlen(longestSituation) + 3);
    instruction= newwin(3, strlen(instrMsg) + 1, LINES - 5, COLS - strlen(instrMsg) - 5);
    

    start_color();
    /*use_default_colors();*/
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    init_pair(8, COLOR_BLUE, COLOR_BLACK);
    init_pair(9, COLOR_GREEN, COLOR_BLACK);

    refresh();

    box(maintitle,0,0);


    wattron(maintitle, COLOR_PAIR(1));
    mvwprintw(maintitle, 1, (COLS / 2) - (strlen(title) / 2), title);
    wattroff(maintitle, COLOR_PAIR(1));


    wattron(titles, A_UNDERLINE);
    mvwprintw(titles, 1, (COLS/2) - 14 - MARGIN, "Cartes des lieux");
    mvwprintw(titles, 1, (COLS/2) + MARGIN, "Légende");
    wattroff(titles, A_UNDERLINE);

    wattron(titles2, A_UNDERLINE);
    mvwprintw(titles2, 1, (COLS/2) - 40 - MARGIN, "Cartes des citoyens");
    mvwprintw(titles2, 1, (COLS/2) + MARGIN, "Evolution de l'épidémie");
    wattroff(titles2, A_UNDERLINE);

   


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



    mvwprintw(situation, 0, 0, longestSituation);
    mvwprintw(situation, 2, 0, "Personnes malades");
    mvwprintw(situation, 4, 0, "Personnes décédées");
    mvwprintw(situation, 6, 0, "Cadavres brûlés");

    wattron(people, COLOR_PAIR(9));
    mvwprintw(people, 0, 0, "10");
    wattroff(people, COLOR_PAIR(9));
    wattron(people, COLOR_PAIR(6));
    mvwprintw(people, 2, 0, "10");
    wattroff(people, COLOR_PAIR(6));
    wattron(people, COLOR_PAIR(8));
    mvwprintw(people, 4, 0, "10");
    wattroff(people, COLOR_PAIR(8));
    wattron(people, COLOR_PAIR(7));
    mvwprintw(people, 6, 0, "10");
    wattroff(people, COLOR_PAIR(7));



    wattron(citizens, COLOR_PAIR(9));
    for(int i=0; i<7; i++) {
        for(int j=0; j<7; j++) {
            mvwprintw(citizens, i*3, j*6, "2");
        }        
    }

    wattron(citizens, COLOR_PAIR(8));
    for(int i=0; i<7; i++) {
        for(int j=0; j<7; j++) {
            mvwprintw(citizens, i*3+1, j*6, "0");
        }        
    }

    wattron(citizens, COLOR_PAIR(6));
    for(int i=0; i<7; i++) {
        for(int j=0; j<7; j++) {
            mvwprintw(citizens, i*3, j*6+1, "1");
        }        
    }

    wattron(citizens, COLOR_PAIR(7));
    for(int i=0; i<7; i++) {
        for(int j=0; j<7; j++) {
            mvwprintw(citizens, i*3+1, j*6+1, "3");
        }        
    }



    wattron(instruction, COLOR_PAIR(1));
    mvwprintw(instruction, 0, 0, instrMsg);
    wattroff(instruction, COLOR_PAIR(1));





    wrefresh(maintitle);
    wrefresh(titles);
    wrefresh(places);
    wrefresh(legend);
    wrefresh(titles2);
    wrefresh(citizens);
    wrefresh(situation);
    wrefresh(people);
    wrefresh(instruction);
   

    getch();

    delwin(maintitle);
    delwin(titles);
    delwin(places);
    delwin(legend);
    delwin(titles2);
    delwin(citizens);
    delwin(situation);
    delwin(people);
    delwin(instruction);
    

    endwin();
 
    free(maintitle);
    free(titles);
    free(places);
    free(legend);
    free(titles2);
    free(citizens);
    free(situation);
    free(people);
    free(instruction);
    

 
    return 0;
}
