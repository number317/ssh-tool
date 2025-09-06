#ifndef _HOST_H
#define _HOST_H
typedef struct host_ {
    const char *hostname;
    const char *ip;
    const char *port;
    const char *use_key;
    const char *username;
    const char *password;
    const char *comment;
    const char *proxy_hostname;
    struct host_ *proxy_host;
} host;

void print_host(host*);
char *build_command(const char *fmt, ...);
char *get_proxy_command(host *);
char *get_connect_command(host *);
#endif
