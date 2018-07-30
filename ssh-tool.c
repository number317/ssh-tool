# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<ncurses.h>

#define MAXROW 500
#define MAXCOL 500

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
            case '\r':
                connect();
                reload();
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
    mvaddstr(winrow, 0, cmdoutlines[clinenum]);
    attroff(A_REVERSE);
    refresh();
}

void getConf(){
    FILE *p;
    char ln[MAXCOL];
    int row;
    char *resource=(char*)malloc(sizeof(char)*50);
    resource=getenv("HOME");
    p = fopen(strcat(resource,"/.config/ssh-tool/server.txt"), "r");
    if(p==NULL){
        printf("Resource file %s dose't exists!\n", resource);
        endwin();
        exit(0);
    }
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

void reload(){
    getConf();
    refresh();
    showlastpart();
}

void connect(){
    refresh();
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
    char *temp[] = {"sshpass -p ", 
    password, " ssh ",
    username, "@", ip,
    " -p ",
    port};
    char *result;
    result = (char *)malloc(100*sizeof(char));
    result[0]='\0';
    for(i=0; i<8; i++){
        strcat(result, temp[i]);
    }
    printf("connect to %s...\n", hostname);
    system(result);
}

