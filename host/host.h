#ifndef _HOST_H
#define _HOST_H
typedef struct _host {
    const char *hostname;
    const char *ip;
    const char *port;
    const char *use_key;
    const char *username;
    const char *password;
    const char *comment;
} host;

void print_host(host*);
#endif
