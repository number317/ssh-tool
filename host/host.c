#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "host.h"
#include "../utility/utility.h"

void print_host(host *host) {
    printf("{hostname: %s,"
            "ip: %s,"
            "port: %s,"
            "use_key: %d,"
            "username: %s,"
            "password: %s,"
            "proxy_hostname: %s,"
            "comment: %s}\n",
            host->hostname,
            host->ip,
            host->port,
            host->use_key,
            host->username,
            host->password,
            host->proxy_hostname,
            host->comment);
}

char *build_command(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    int len = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    error_handle(len < 0, "command length error!");

    char *command = malloc(sizeof(char) * (len + 1));
    error_handle(!command, "command malloc failed!");

    va_start(ap, fmt);
    vsnprintf(command, len + 1, fmt, ap);
    va_end(ap);

    return command;
}

char *get_proxy_command(host *h) {
    char *proxy_command;
    if (h->use_key)
        proxy_command = build_command("ssh -i %s -W %%h:%%p %s@%s -p %s",
                h->password,
                h->username,
                h->ip,
                h->port);
    else
        proxy_command = build_command("SSHPASS='%s' sshpass -e ssh -W %%h:%%p %s@%s -p %s",
                h->password,
                h->username,
                h->ip,
                h->port);
    return proxy_command;
}

char *get_connect_command(host *h) {
    char *command;
    if (h->proxy_host != NULL) {
        char *proxy_command = get_proxy_command(h->proxy_host);
        if (h->use_key)
            command = build_command("ssh -o StrictHostKeyChecking=accept-new -o ProxyCommand=\"%s\" -i %s %s@%s -p %s",
                    proxy_command,
                    h->password,
                    h->username,
                    h->ip,
                    h->port);
        else
            command = build_command("sshpass -e ssh -o StrictHostKeyChecking=accept-new -o ProxyCommand=\"%s\" %s@%s -p %s",
                    proxy_command,
                    h->username,
                    h->ip,
                    h->port);
        free(proxy_command);
    } else {
        if (h->use_key)
            command = build_command("ssh -o StrictHostKeyChecking=accept-new -i %s %s@%s -p %s",
                    h->password,
                    h->username,
                    h->ip,
                    h->port);
        else
            command = build_command("sshpass -e ssh -o StrictHostKeyChecking=accept-new %s@%s -p %s",
                    h->username,
                    h->ip,
                    h->port);
    }
    return command;
}
