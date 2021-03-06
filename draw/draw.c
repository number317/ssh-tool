#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "../utility/utility.h"
#include "draw.h"

void show(conf_set *confs, int current_row, int show_password, int old_page) {

    int current_page = current_row/confs->hosts_perpage;
    if(old_page!=current_page)
        clear();

    // make sure start from first line to draw
    mvprintw(0, 0, "%-15s", confs->header[0]);
    printw("%-18s", confs->header[1]);
    printw("%-8s", confs->header[2]);
    printw("%-15s", confs->header[3]);
    printw("%-22s", confs->header[4]);
    printw("%-20s", confs->header[5]);

    mvprintw(1, 0, "%s\n", confs->seperation);

    int start = current_page*confs->hosts_perpage;
    int end = min(start + confs->hosts_perpage, confs->hosts_length);
    for(int i=start; i<end; i++) {
        if(i==current_row) attron(A_REVERSE);
        printw("%-15.14s%-18.17s%-8.7s%-15.14s",
                confs->hosts[i]->hostname,
                confs->hosts[i]->ip,
                confs->hosts[i]->port,
                confs->hosts[i]->username);
        if(!show_password) printw("%-22.21s", "************");
        else printw("%-22.21s", confs->hosts[i]->password);
        printw("%.20s\n", confs->hosts[i]->comment);
        attroff(A_REVERSE);
    }
    int status_line = min(confs->hosts_perpage+5, confs->hosts_length+5);
    if(confs->pages>0)
        mvprintw(status_line, 80, "page %d of %d", current_page+1, confs->pages+1);
    curs_set(0);
    refresh();
}
