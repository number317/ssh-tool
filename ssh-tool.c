#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <libconfig.h>
#include "config/config.h"
#include "draw/draw.h"

int main(int argc, char *argv[]){
    /*{{{ init config */
    char *config_file = (char*)calloc(100, sizeof(char));
    if(argc!=2)
        snprintf(config_file, 100, "%s/.config/ssh-tool/hosts.cfg", getenv("HOME"));
    else
        snprintf(config_file, 100, "%s", argv[1]);

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
    int current_row=0;
    int show_password=0;
    /*}}}*/

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

    show(confs, current_row, show_password);

    /*{{{ keyboard event */
    char operator;
    char *command = (char*)calloc(100, sizeof(char));
    while((operator=getchar())!='q')
    {
        switch(operator)
        {
            case 'e':
                endwin();
                if(!getenv("EDITOR")) {
                    fprintf(stderr, "Your editor haven't been set!\n"
                            "3 seconds later will go back...\n");
                    sleep(3);
                }
                else {
                    snprintf(command, 100, "%s %s", getenv("EDITOR"), config_file);
                    system(command);
                    clean_hosts_content(confs->hosts, confs->hosts_length);
                    confs = get_conf_set(&config, config_file, confs);
                    current_row=0;
                    show_password=0;
                }
                show(confs, current_row, show_password);
                break;
            case 'j':
                current_row= current_row>=confs->hosts_length-2 ?
                    confs->hosts_length-1 : current_row+1;
                show(confs, current_row, show_password);
                break;
            case 'k':
                current_row= current_row<=1 ? 0 : current_row-1;
                show(confs, current_row, show_password);
                break;
            case 'r':
                endwin();
                clear();
                clean_hosts_content(confs->hosts, confs->hosts_length);
                confs = get_conf_set(&config, config_file, confs);
                current_row=0;
                show_password=0;
                show(confs, current_row, show_password);
                break;
            case 's':
                show_password=(show_password+1)%2;
                show(confs, current_row, show_password);
                break;
            case 'G':
                current_row=confs->hosts_length-1;
                show(confs, current_row, show_password);
                break;
            case '\r':
                endwin();
                snprintf(command, 100, "sshpass -p %s ssh %s@%s -p %s",
                        confs->hosts[current_row]->password,
                        confs->hosts[current_row]->username,
                        confs->hosts[current_row]->ip,
                        confs->hosts[current_row]->port);
                printf("connect to %s(%s)...\n",
                        confs->hosts[current_row]->hostname,
                        confs->hosts[current_row]->ip);
                system(command);
                show(confs, current_row, show_password);
                break;
            default:
                break;
        }
    }/*}}}*/

    endwin();
    free(config_file); config_file=NULL;
    free(command); command=NULL;
    clean_conf_set(confs);
    config_destroy(config);
    free(config);
    config=NULL;
    return 0;
}
