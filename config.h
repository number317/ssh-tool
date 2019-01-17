#include <libconfig.h>
#include "host.h"

char** get_header(config_t *, char **, int);
char* get_seperation(config_t *, char *, int);
host** get_hosts(config_t*, host **, int);
