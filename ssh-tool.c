#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <libconfig.h>
#include "config.h"
#include "draw.h"

int main(int argc, char *argv[]){

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

/*{{{ init config */
    char *config_file = (char*)calloc(100, sizeof(char));
    if(argc!=2)
        snprintf(config_file, 100, "%s/.config/ssh-tool/hosts.cfg", getenv("HOME"));
    else
        snprintf(config_file, 100, argv[1]);
    config_t *config = malloc(sizeof(config_t));

    config_init(config);
    config_read_file(config, config_file);
    
    /** header */
    int header_length = config_setting_length(
            config_lookup(config, "header")
            );//header_length should be 6
    char **header = (char**)calloc(header_length, sizeof(char*));
    get_header(config, header, header_length);

    /** seperation */
    int seperation_length = config_setting_get_int(
            config_lookup(config, "seperation_length")
            );
    char *seperation = (char*)calloc(seperation_length, sizeof(char));
    get_seperation(config, seperation, seperation_length);

    /** hosts */
    int hosts_length = config_setting_length(
            config_lookup(config, "hosts")
            );
    host **hosts = (host**)calloc(hosts_length, sizeof(host*));
    get_hosts(config, hosts, hosts_length);
    int current_row=0;
    int show_password=0;
/*}}}*/

    show(header, seperation, hosts, hosts_length, current_row, show_password);

/*{{{ keyboard event */
    char operator;
    char *command = (char*)calloc(100, sizeof(char));
    while((operator=getchar())!='q')
    {
        switch(operator)
        {
            case 'e':
                endwin();
                char *editor = (char*)calloc(10, sizeof(char));
                if(!(editor = getenv("EDITOR"))) {
                    fprintf(stderr, "Your editor haven't been set!\n"
                            "3 seconds later will go back...\n");
                    sleep(3);
                }
                else {
                    snprintf(command, 100, "%s %s", editor, config_file);
                    system(command);

                    config_init(config);
                    config_read_file(config, config_file);
                    header_length = config_setting_length(
                            config_lookup(config, "header")
                            );
                    header = (char**)calloc(header_length, sizeof(char*));
                    get_header(config, header, header_length);

                    seperation_length = config_setting_get_int(
                            config_lookup(config, "seperation_length")
                            );
                    seperation = (char*)calloc(seperation_length, sizeof(char));
                    get_seperation(config, seperation, seperation_length);

                    hosts_length = config_setting_length(
                            config_lookup(config, "hosts")
                            );
                    hosts = (host**)calloc(hosts_length, sizeof(host*));
                    get_hosts(config, hosts, hosts_length);
                    current_row=0;
                    show_password=0;
                }
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                free(editor); editor=NULL;
                break;
            case 'j':
                current_row= current_row>=hosts_length-2 ? hosts_length-1 : current_row+1;
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                break;
            case 'k':
                current_row= current_row<=1 ? 0 : current_row-1;
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                break;
            case 'r':
                endwin();
                clear();
                config_init(config);
                config_read_file(config, config_file);
                header_length = config_setting_length(
                        config_lookup(config, "header")
                        );
                header = (char**)calloc(header_length, sizeof(char*));
                get_header(config, header, header_length);

                seperation_length = config_setting_get_int(
                        config_lookup(config, "seperation_length")
                        );
                seperation = (char*)calloc(seperation_length, sizeof(char));
                get_seperation(config, seperation, seperation_length);

                hosts_length = config_setting_length(
                        config_lookup(config, "hosts")
                        );
                hosts = (host**)calloc(hosts_length, sizeof(host*));
                get_hosts(config, hosts, hosts_length);
                current_row=0, show_password=0;
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                break;
            case 's':
                show_password=(show_password+1)%2;
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                break;
            case 'G':
                current_row=hosts_length-1;
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                break;
            case '\r':
                endwin();
                snprintf(command, 100, "sshpass -p %s ssh %s@%s -p %s",
                        hosts[current_row]->password,
                        hosts[current_row]->username,
                        hosts[current_row]->ip,
                        hosts[current_row]->port);
                printf("connect to %s(%s)...\n", hosts[current_row]->hostname,hosts[current_row]->ip);
                system(command);
                show(header, seperation, hosts, hosts_length, current_row, show_password);
                break;
            default:
                break;
        }
    }/*}}}*/

    endwin();
    free(config_file); config_file=NULL;
    free(command); command=NULL;
    free(header); header=NULL;
    free(seperation); seperation=NULL;
    free(hosts); hosts=NULL;
    config_destroy(config);
    return 0;
}
