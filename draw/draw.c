#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "draw.h"

void show(
        char **header,
        char *seperation,
        host **hosts, int hosts_length,
        int current_row,
        int show_password
        ) {

    // make sure start from first line to draw
    mvprintw(0, 0, "%-15s", header[0]);
    printw("%-18s", header[1]);
    printw("%-8s", header[2]);
    printw("%-15s", header[3]);
    printw("%-22s", header[4]);
    printw("%-20s", header[5]);

    mvprintw(1, 0, "%s\n", seperation);
    for(int i=0; i<hosts_length; i++) {
        if(i==current_row)
            attron(A_REVERSE);
        printw("%-15.14s%-18.17s%-8.7s%-15.14s",
                hosts[i]->hostname,
                hosts[i]->ip,
                hosts[i]->port,
                hosts[i]->username);
        if(!show_password)
            printw("%-22.21s", "************");
        else
            printw("%-22.21s", hosts[i]->password);
        printw("%.20s\n", hosts[i]->comment);
        attroff(A_REVERSE);
    }
    curs_set(0);
    refresh();
}
