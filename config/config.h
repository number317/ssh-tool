#ifndef _CONFIG_H
#define _CONFIG_H
#include <libconfig.h>
#include "../host/host.h"

// conf_set struct used to wrap arguement
typedef struct _conf_set {
    char **header;
    int header_length;
    char *seperation;
    int seperation_length;
    host **hosts;
    int hosts_length;
} conf_set;

void error_handle(int, char *);
config_t* set_config_file(config_t *, char *);
int get_length(config_t *, char *);
char** get_header(config_t *, char **, int);
char* get_seperation(config_t *, char *, int);
host** get_hosts(config_t*, host **, int);
void clean_hosts_content(host **, int);
conf_set* get_conf_set(config_t **, char *, conf_set *);
void clean_conf_set(conf_set *);

#endif
