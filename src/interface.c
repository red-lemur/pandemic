#include <ncurses.h>

int main(void)
{	

    WINDOW *haut, *bas;
    
    initscr();
    haut= subwin(stdscr, LINES / 10, COLS, 0, 0);        // Créé une fenêtre de 'LINES / 2' lignes et de COLS colonnes en 0, 0
    bas= subwin(stdscr, LINES * 9/10, COLS, LINES / 10, 0); // Créé la même fenêtre que ci-dessus sauf que les coordonnées changent
    

    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    attroff(COLOR_PAIR(1));

    mvwprintw(haut, 1, 1, "Ceci est la fenetre du haut");
    mvwprintw(bas, 1, 1, "Ceci est la fenetre du bas");
    
    wrefresh(haut);
    wrefresh(bas);
    
    getch();
    endwin();
 
    free(haut);
    free(bas);
 
    return 0;
}
