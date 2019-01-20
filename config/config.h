#include <libconfig.h>
#include "../host/host.h"

void error_handle(int, char *);
config_t* set_config_file(config_t *, char *);
int get_length(config_t *, char *);
char** get_header(config_t *, char **, int);
char* get_seperation(config_t *, char *, int);
host** get_hosts(config_t*, host **, int);
