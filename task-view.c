# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<sys/types.h>
# include<signal.h>
# include<ncurses.h>

#define MAXROW 1000
#define MAXCOL 500

WINDOW *scrn;

char cmdoutlines[MAXROW][MAXCOL];

int ncmdlines,
    nwinlines,

    winrow,
    cmdstartrow,
    cmdlastrow;

void highlight(){
    int clinenum;
    attron(A_REVERSE);
    clinenum = cmdstartrow + winrow;
    mvaddstr(winrow, 0, cmdoutlines[clinenum]);
    attroff(A_REVERSE);
    refresh();
}

void runpsax(){
    FILE *p;
    char ln[MAXCOL];
    int row, tmp;
    p = popen("ps ax", "r");
    printf("hello\n");

    for (row = 0; row < MAXROW; row++){
        tmp = fgets(ln, MAXCOL, p);
        if (tmp == NULL) break;
        strncpy(cmdoutlines[row], ln, COLS);
        cmdoutlines[row][MAXCOL-1] = 0;
    }
    ncmdlines = row;
    pclose(p);
}

void showlastpart(){
    int row;
    clear();
    if (ncmdlines <= LINES){
        cmdstartrow = 0;
        nwinlines = ncmdlines;
    }
    else {
        cmdstartrow = ncmdlines -LINES;
        nwinlines = LINES;
    }
    cmdlastrow = cmdstartrow + nwinlines - 1;
    for (row = cmdstartrow, winrow = 0; row <= cmdlastrow; row++, winrow++)
        mvaddstr(winrow, 0, cmdoutlines[row]);
    refresh();
    winrow--;
    highlight();
}

void updown(int inc){
    int tmp = winrow + inc;
    if (tmp >= 0 && tmp < LINES) {
        mvaddstr(winrow, 0, cmdoutlines[cmdstartrow+winrow]);
        winrow = tmp;
        highlight();
    }
}

void rerun(){
    runpsax();
    showlastpart();
}

void prockill(){
    char *pid;
    pid = strtok(cmdoutlines[cmdstartrow+winrow], " ");
    kill(atoi(pid),9);
    rerun();
}

int main(){
    char c;
    scrn = initscr();
    noecho();
    cbreak();
    runpsax();
    showlastpart();
    while (1) {
        c = getchar();
        if ( c == 'k') updown(-1);
        else if ( c == 'j') updown(1);
        else if ( c == 'r') rerun();
        else if ( c == 'q') prockill();
        else break;
    }
    endwin();
    return 0;
}
