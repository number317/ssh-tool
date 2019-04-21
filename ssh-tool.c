#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <libconfig.h>
#include "draw/draw.h"
#include "ssh-tool.h"

int main(int argc, char *argv[]){
    /*{{{ args handler */
    char *config_file = (char*)calloc(100, sizeof(char));
    switch(argc){
        case 1:
            snprintf(config_file, 100, "%s/.config/ssh-tool/hosts.cfg",
                    getenv("HOME"));
            break;
        case 2:
            if(access(argv[1], R_OK)==0)
                snprintf(config_file, 100, "%s", argv[1]);
            else{
                fprintf(stderr,
                        "config file %s doesn't exist"
                        " or you have wrong permission\n", argv[1]);
                usage();
                exit(1);
            }
            break;
        default:
            usage();
            exit(1);
    }
    /*}}}*/

    /*{{{ init config */
    config_t *config = malloc(sizeof(config_t));

    config_init(config);
    config = set_config_file(config, config_file);

    conf_set *confs = malloc(sizeof(conf_set));

    confs->header_length = get_length(config, "header");
    confs->seperation_length = get_length(config, "seperation_length");
    confs->hosts_length = get_length(config, "hosts");

    confs->header = (char**)malloc(sizeof(char*)*confs->header_length);
    confs->seperation = (char*)malloc(sizeof(char)*confs->seperation_length);
    confs->hosts = (host**)malloc(sizeof(host*)*confs->hosts_length);

    confs = get_conf_set(&config, config_file, confs);
    int pages = confs->hosts_length%confs->hosts_perpage==0?
        confs->hosts_length/confs->hosts_perpage-1 :
        confs->hosts_length/confs->hosts_perpage;
    int current_page = 0;
    int current_row = 0;
    int row_start = 0;
    int row_end = 0;
    int status_line = confs->hosts_length > confs->hosts_perpage ?
                    confs->hosts_perpage+5 : confs->hosts_length+5;
    int show_password = 0;
    /*}}}*/

    /*{{{ init screen */
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    /** noecho(); */
    show(confs, current_page, current_row, show_password);
    /*}}}*/

    /*{{{ keyboard event */
    char operator;
    char *command = (char*)calloc(100, sizeof(char));
    while((operator=getchar())!='q')
    {
        switch(operator)
        {
            case 'e':
                endwin();
                clear();
                if(!getenv("EDITOR")) {
                    fprintf(stderr, "%s","Your editor haven't been set!\n"
                            "Please set 'EDITOR' environment variable first.\n"
                            "press 'Enter' to continue ...");
                    getchar();
                }
                else {
                    snprintf(command, 100, "%s %s",
                            getenv("EDITOR"), config_file);
                    system(command);
                    clean_hosts_content(confs->hosts, confs->hosts_length);
                    confs = get_conf_set(&config, config_file, confs);
                    pages = confs->hosts_length%confs->hosts_perpage==0?
                        confs->hosts_length/confs->hosts_perpage-1 :
                        confs->hosts_length/confs->hosts_perpage;
                    status_line = confs->hosts_length > confs->hosts_perpage ?
                        confs->hosts_perpage+5 : confs->hosts_length+5;
                    current_page = 0;
                    current_row = 0;
                    show_password = 0;
                }
                show(confs, current_page, current_row, show_password);
                break;
            case 'j':
                row_end =
                    (current_page+1)*confs->hosts_perpage>confs->hosts_length ?
                    confs->hosts_length :
                    (current_page+1)*confs->hosts_perpage;
                current_row = current_row>=row_end-1 ?
                    row_end -1 : current_row+1;
                show(confs, current_page, current_row, show_password);
                break;
            case 'k':
                row_start = current_page*confs->hosts_perpage;
                current_row= current_row<=row_start ?
                    row_start : current_row-1;
                show(confs, current_page, current_row, show_password);
                break;
            case 'J':
                current_page= current_page+1>=pages ?
                     pages : current_page+1;
                current_row = current_page*confs->hosts_perpage;
                clear();
                show(confs, current_page, current_row, show_password);
                break;
            case 'K':
                current_page= current_page-1<=0 ?
                     0 : current_page-1;
                current_row = current_page*confs->hosts_perpage;
                clear();
                show(confs, current_page, current_row, show_password);
                break;
            case '/':
                mvprintw(status_line, 0, "%c", operator);
                show(confs, current_page, current_row, show_password);
                mvgetstr(status_line, 1, command);
            case 'n':
            case 'N':
                current_row = get_match_row(confs, current_row,
                        operator=='N' ? -1 : 1, command);
                int old_current_page = current_page;
                if(confs->hosts_perpage >= confs->hosts_length
                        || current_row < confs->hosts_perpage)
                    current_page = 0;
                else
                    current_page = current_row%confs->hosts_perpage==0 ?
                        current_row/confs->hosts_perpage :
                        current_row/confs->hosts_perpage;
                if(current_page != old_current_page)
                    clear();
                mvprintw(status_line, 0, "%s", "                      ");
                show(confs, current_page, current_row, show_password);
                break;
            case 'r':
                endwin();
                clear();
                clean_hosts_content(confs->hosts, confs->hosts_length);
                confs = get_conf_set(&config, config_file, confs);
                current_page=0;
                current_row=0;
                show_password=0;
                status_line = confs->hosts_length > confs->hosts_perpage ?
                    confs->hosts_perpage+5 : confs->hosts_length+5;
                show(confs, current_page, current_row, show_password);
                break;
            case 's':
                show_password=(show_password+1)%2;
                show(confs, current_page, current_row, show_password);
                break;
            case '0':
                current_row = current_page*confs->hosts_perpage;
                show(confs, current_page, current_row, show_password);
                break;
            case '1': case '2': case '3': case '4': case '5': case '6':
            case '7': case '8': case '9':
                current_page = operator-49>pages ? pages : operator-49;
                current_row = current_page*confs->hosts_perpage;
                clear();
                show(confs, current_page, current_row, show_password);
                break;
            case '$':
                current_row =
                    (current_page+1)*confs->hosts_perpage>confs->hosts_length ?
                    confs->hosts_length-1 :
                    (current_page+1)*confs->hosts_perpage-1;
                show(confs, current_page, current_row, show_password);
                break;
            case 'G':
                current_row = confs->hosts_length-1;
                current_page = pages;
                if(pages>0)
                    clear();
                show(confs, current_page, current_row, show_password);
                break;
            case '\r':
                endwin();
                login(confs->hosts[current_row], command);
                show(confs, current_page, current_row, show_password);
                break;
            default:
                break;
        }
    }/*}}}*/

/*{{{ clean */
    endwin();
    free(config_file); config_file=NULL;
    free(command); command=NULL;
    clean_conf_set(confs);
    config_destroy(config);
    free(config);
    config=NULL;
    return 0;
}
/*}}}*/

/*{{{ function usage*/
void usage(){
    printf("usage: ssh-tool <config_file>\n");
    printf("shortcut:\n");
    printf("   'j': move down\n");
    printf("   'k': move up\n");
    printf("   'J': next page\n");
    printf("   'K': prev page\n");
    printf("   'r': refresh, move to first row of first page\n");
    printf("   'G': move to last row of last page\n");
    printf("   '1-9': move to page 1-9\n");
    printf("   '0': move to first row of current page\n");
    printf("   '$': move to last row of current page\n");
    printf("   '/': search\n");
    printf("   'n': search next\n");
    printf("   'N': search prev\n");
    printf("   's': toggle password\n");
    printf("   'Enter': connect\n");
    printf("   'q': quit\n");
}/*}}}*/

/*{{{ function login */
void login(host *h, char *command){
    if(strcmp(h->use_key, "false")==0)
        snprintf(command, 100, "sshpass -p %s ssh %s@%s -p %s",
                h->password,
                h->username,
                h->ip,
                h->port);
    else
        snprintf(command, 100, "ssh -i %s %s@%s -p %s",
                h->password,
                h->username,
                h->ip,
                h->port);
    printf("connect to %s(%s)...\n",
            h->hostname,
            h->ip);
    system(command);
}
/*}}}*/

/*{{{ function get_match_row */
int get_match_row(conf_set *confs,
        int current_row, int direction, char *pattern) {
    for(int i=current_row+direction; i<confs->hosts_length && i>0; i=i+direction)
        if(strstr(confs->hosts[i]->hostname, pattern)!=NULL
                || strstr(confs->hosts[i]->ip, pattern)!=NULL
                || strstr(confs->hosts[i]->username, pattern)!=NULL
                || strstr(confs->hosts[i]->comment, pattern)!=NULL){
            current_row=i;
            break;
        }
    return current_row;
}/*}}}*/
