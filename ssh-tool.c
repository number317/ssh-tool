# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<sys/types.h>
# include<signal.h>
# include<ncurses.h>

#define MAXROW 1000
#define MAXCOL 500

WINDOW *scrn;

typedef struct {
    char *hostname;
    char *ip;
    char *username;
    char *password;
    char *intro;
}host;

host temp1={"solar", "104.128.226.70", "root", "**0x0112358D", "openvpn server"};
host temp2={"eco", "49.4.2.233", "root", "Handhand300170", "eco server"};

char cmdoutlines[MAXROW][MAXCOL];

int ncmdlines,
    nwinlines,

    winrow,
    cmdstartrow,
    cmdlastrow;

void reverse(){
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
    /** p = popen("ps ax", "r"); */
    p = popen("cat ./server.txt", "r");
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
    printw("================================================================\n");
    for (row = cmdstartrow, winrow = 0; row <= cmdlastrow; row++, winrow++)
        mvaddstr(winrow, 0, cmdoutlines[row]);
    refresh();
    winrow--;
    reverse();
}

void updown(int inc){
    int tmp = winrow + inc;
    if (tmp >= 0 && tmp < LINES) {
        mvaddstr(winrow, 0, cmdoutlines[cmdstartrow+winrow]);
        winrow = tmp;
        reverse();
    }
}

void rerun(){
    runpsax();
    showlastpart();
}

void connect(){
    endwin();
    char *temp[] = {"sshpass -p ", temp1.hostname, " ssh ", temp1.username, "@", temp1.hostname};
    char *result=NULL;
    for(int i=0; i<6; i++){
        printf("%s\n", temp[i]);
        strcat(result, temp[i]);
    }
    printf("%s\n", result);
    //system("sshpass -p ssh root@49.4.2.233");
    getchar();
    rerun();
}

void prockill(){
    char *pid;
    pid = strtok(cmdoutlines[cmdstartrow+winrow], " ");
    /** kill(atoi(pid),9); */
    rerun();
    printf("=============================================\n");
    printf("%s\n", pid);
    printf("=============================================\n");
}

int main(){
    char c;
    scrn = initscr();
    noecho();
    cbreak();
    runpsax();
    showlastpart();
    while (c != 'q') {
        c = getchar();
        switch(c){
            case 'k':
                updown(-1);
                break;
            case 'j':
                updown(1);
                break;
            case 'r':
                rerun();
                break;
            case '\r':
                /** prockill(); */
                connect();
                break;
            case 'q':
                break;
            default:
                ;
        }
    }
    endwin();
    return 0;
}
