#include <stdio.h>
#include "host.h"

host* initHost(host* h, char* group, char* hostname, char* ip, char* port, char* user, char* password, char* intro){
    h->group=group;
    h->hostname=hostname;
    h->ip=ip;
    h->port=port;
    h->user=user;
    h->password=password;
    h->intro=intro;
    return h;
}

void printHost(host* host){
    if(host==NULL)
        printf("null\n");
    else
        printf("{group:%s, hostname:%s, ip:%s, port:%s, user:%s, password:%s, intro:%s}\n",
                host->group, host->hostname, host->ip, host->port, host->user, host->password, host->intro);
}