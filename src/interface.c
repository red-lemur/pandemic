#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{	

    WINDOW *haut, *bas;
    char *title = "Simulation épidémie";
    
    initscr();
    haut= newwin(LINES / 10, COLS, 0, 0);        // Créé une fenêtre de 'LINES / 2' lignes et de COLS colonnes en 0, 0
    bas= newwin(LINES * 9/10, COLS, LINES / 10, 0); // Créé la même fenêtre que ci-dessus sauf que les coordonnées changent
    

    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_GREEN);
    

    refresh();


    attron(COLOR_PAIR(1));

    mvwprintw(haut, 0, (COLS / 2) - (strlen(title) / 2), title);

    attroff(COLOR_PAIR(1));

    mvwprintw(bas, 1, 1, "Ceci est la fenetre du bas");

    
    
    wrefresh(haut);
    wrefresh(bas);
    
    getch();
    delwin(haut);
    delwin(bas);
    endwin();
 
    free(haut);
    free(bas);
 
    return 0;
}
