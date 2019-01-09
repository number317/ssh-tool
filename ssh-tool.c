#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>

#define MAXROW BUFSIZ
#define MAXCOL BUFSIZ

WINDOW *scrn;

char cmdoutlines[MAXROW][MAXCOL];

int ncmdlines,
    nwinlines,
    winrow,
    cmdstartrow,
    cmdlastrow;

void reverse();
void getConf();
void getConf();
void showlastpart();
void updown();
void reload();
void connect();

int main(){
    char c;
    setlocale(LC_ALL,"");
    scrn = initscr();
    noecho();
    cbreak();
    getConf();
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
                reload();
                break;
            case 'e':
                system("vim ~/.config/ssh-tool/server.txt");
                reload();
                break;
            case '\r':
                connect();
                reload();
                break;
            case 'G':
                for(int i=0; i<cmdlastrow; i++)
                    updown(1);
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

void reverse(){
    int clinenum;
    attron(A_REVERSE);
    clinenum = cmdstartrow + winrow;
    mvaddnstr(winrow, 0, cmdoutlines[clinenum], strlen(cmdoutlines[clinenum])-2);
    attroff(A_REVERSE);
    refresh();
}

void getConf(){
    FILE *p;
    char ln[MAXCOL];
    int row;
    char *resource=(char*)malloc(sizeof(char)*50);
    resource=strcpy(resource, getenv("HOME"));
    p = fopen(strcat(resource,"/.config/ssh-tool/server.txt"), "r");
    if(p==NULL){
        printf("Resource file %s dose't exists!\n", resource);
        endwin();
        exit(0);
    }
    for (row = 0; row < MAXROW; row++){
        if(fgets(ln, MAXCOL, p) != NULL){
            strncpy(cmdoutlines[row], ln, strlen(ln));
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
    winrow=2;
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

void reload(){
    getConf();
    refresh();
    showlastpart();
}

void connect(){
    endwin();
    char *password, *username, *ip, *port, *hostname;
    char *str=cmdoutlines[cmdstartrow+winrow];
    char *list=NULL;
    int i=0;
    list=strtok(str," ");
    hostname=list;
    while(list != NULL){
        switch(i){
            case 0:
                list=strtok(NULL, " ");
                ip=list;
                break;
            case 1:
                list=strtok(NULL, " ");
                port=list;
                break;
            case 2:
                list=strtok(NULL, " ");
                username=list;
                break;
            case 3:
                list=strtok(NULL, " ");
                password=list;
                break;
            default:
                list=strtok(NULL, " ");
        }
        i++;
    }
    char *result=malloc(100*sizeof(char));
    sprintf(result, "sshpass -p %s ssh %s@%s -p %s", password, username, ip, port);
    printf("connect to %s...\n", hostname);
    system(result);
}
