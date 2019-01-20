#include <stdio.h>
#include "host.h"

void print_host(host *host) {
    printf("{hostname: %s,"
            "ip: %s,"
            "port: %s,"
            "username: %s,"
            "password: %s,"
            "comment: %s}\n",
            host->hostname,
            host->ip,
            host->port,
            host->username,
            host->password,
            host->comment);
}
