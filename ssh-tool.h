#ifndef _SSH_TOOL_H
#define _SSH_TOOL_H
#include "./config/config.h"
#include "./host/host.h"

void usage();
int row_up_down(conf_set *, int, char);
int page_up_down(conf_set *, int, char);
void login(host *, char *);
int get_match_row(conf_set *, int, int, char *);

#endif
