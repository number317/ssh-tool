# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include <yaml.h>
# include<sys/types.h>
# include<signal.h>
# include<ncurses.h>

#define MAXROW 1000
#define MAXCOL 500

WINDOW *scrn;

typedef struct {
    char *groupname;
    char *hostname;
    char *ip;
    char *username;
    char *password;
    char *intro;
}host;

const host temp1={"all", "solar", "104.128.226.70", "root", "**0x0112358D", "openvpn server"};
const host temp2={"all", "eco", "49.4.2.233", "root", "Handhand300170", "eco server"};

host all[]={temp1,temp2};

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
    int row;
    p = fopen("./server.txt", "r");
    for (row = 0; row < MAXROW; row++){
        if(fgets(ln, MAXCOL, p) != NULL){
            strncpy(cmdoutlines[row], ln, COLS);
            cmdoutlines[row][MAXCOL-1] = 0;
        } else break;
    }
    ncmdlines = row;
    fclose(p);
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
    reverse();
}

void updown(int inc){
    int tmp = winrow + inc;
    if (tmp >= 0 && tmp < LINES) {
        mvaddstr(winrow, 0, cmdoutlines[cmdstartrow+winrow]);
        if(tmp <= cmdlastrow && tmp >= cmdstartrow+2){
            winrow = tmp;
        }
        reverse();
    }
}

void rerun(){
    runpsax();
    showlastpart();
}

void connect(){
    refresh();
    endwin();
    char *temp[] = {"sshpass -p ", temp2.password, " ssh ", temp2.username, "@", temp2.ip};
    char *result;
    result = (char *)malloc(100*sizeof(char));
    result[0]='\0';
    int i;
    for(i=0; i<6; i++){
        strcat(result, temp[i]);
    }
    printf("connect to %s...\n", temp1.hostname);
    printf("%s\n", result);
    system(result);
    free(result);
    rerun();
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
