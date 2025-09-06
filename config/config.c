#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "../utility/utility.h"

config_t *set_config_file(config_t *config, char *config_file) {
    if(! config_read_file(config, config_file)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(config),
                config_error_line(config), config_error_text(config));
        config_destroy(config);
        exit(1);
    }
    return config;
}

int get_length(config_t *config, char *setting) {
    config_setting_t *cst = config_lookup(config, setting);
    error_handle(!cst, "Error! Config file doesn't have the header config\n");
    if( config_setting_is_list(cst)
            || config_setting_is_array(cst)
            || config_setting_is_group(cst))
        return config_setting_length(cst);
    else if(config_setting_is_number(cst))
        return config_setting_get_int(cst);
    else
        return -1;
}

/*
 * header is an array used to save table's header
 * header_length is the length of array header and it should not change
 */
char **get_header(
        config_t *config,
        char **header,
        int header_length) {
    config_setting_t *headers;
    error_handle(!(headers=config_lookup(config, "header")),
            "Error! Config file doesn't have the header config\n");
    for(int i=0; i<header_length; i++)
        header[i]=(char *)config_setting_get_string_elem(headers, i);
    return header;
}

/*
 * seperation is a string to save seperation
 */
char *get_seperation(
        config_t *config,
        char *seperation,
        int seperation_length) {
    char *seperation_char = (char *)config_setting_get_string(
            config_lookup(config, "seperation_char"));
    /*
     * make sure seperation has the enough space
     */
    int char_width = strlen(seperation_char);
    char *new_seperation;
    new_seperation = realloc(seperation, seperation_length*char_width+1);
    memset(new_seperation, 0, seperation_length*char_width+1);
    for(int i=0; i<seperation_length;i++)
        snprintf(new_seperation+i*char_width, char_width+1,
                "%s", seperation_char);
    return new_seperation;
}

host **get_hosts(
        config_t *config,
        host **hosts,
        int hosts_length) {
    config_setting_t *host_list = config_lookup(config, "hosts");
    error_handle(!host_list,
            "Error! Config file doesn't have the hosts config\n");
    host **new_hosts;
    new_hosts = realloc(hosts, hosts_length*sizeof(host*));
    for(int i=0; i<hosts_length; i++) {
        new_hosts[i] = (host *)calloc(1, sizeof(host));
        config_setting_t *hosts_item = config_setting_get_elem(host_list, i);
        config_setting_lookup_string(hosts_item, "hostname",
                &(new_hosts[i]->hostname));
        config_setting_lookup_string(hosts_item, "ip", &(new_hosts[i]->ip));
        config_setting_lookup_string(hosts_item, "port",
                &(new_hosts[i]->port));
        config_setting_lookup_string(hosts_item, "use_key",
                &(new_hosts[i]->use_key));
        config_setting_lookup_string(hosts_item, "username",
                &(new_hosts[i]->username));
        config_setting_lookup_string(hosts_item, "password",
                &(new_hosts[i]->password));
        config_setting_lookup_string(hosts_item, "proxy_hostname",
                &(new_hosts[i]->proxy_hostname));
        config_setting_lookup_string(hosts_item, "comment",
                &(new_hosts[i]->comment));
    }
    for (int i = 0; i < hosts_length; i++) {
        if (new_hosts[i]->proxy_hostname != NULL) {
            new_hosts[i]->proxy_host = get_host_by_hostname(new_hosts, hosts_length, new_hosts[i]->proxy_hostname);
        }
    }
    return new_hosts;
}

host *get_host_by_hostname(host **hosts, int hosts_length, const char *hostname) {
    host *result = NULL;
    for(int i=0; i<hosts_length; i++) {
        if(!strcmp(hosts[i]->hostname, hostname)) {
            result = hosts[i];
            break;
        }
    }
    return result;
}

void clean_hosts_content(host **hosts, int hosts_length) {
    for(int i=0; i<hosts_length; i++)
        free(hosts[i]);
}

conf_set *get_conf_set(config_t **config, char *config_file, conf_set *confs) {
    set_config_file(*config, config_file);

    confs->header_length = get_length(*config, "header");
    confs->seperation_length = get_length(*config, "seperation_length");
    confs->hosts_perpage = get_length(*config, "hosts_perpage");
    confs->pages = confs->hosts_length%confs->hosts_perpage==0?
        confs->hosts_length/confs->hosts_perpage-1 :
        confs->hosts_length/confs->hosts_perpage;
    confs->hosts_length = get_length(*config, "hosts");

    confs->header = get_header(*config, confs->header, confs->header_length);
    confs->seperation = get_seperation(*config, confs->seperation,
            confs->seperation_length);
    confs->hosts = get_hosts(*config, confs->hosts, confs->hosts_length);
    return confs;
}

void clean_conf_set(conf_set *confs) {
    free(confs->header);
    confs->header = NULL;
    free(confs->seperation);
    confs->seperation = NULL;
    for(int i=0; i<confs->hosts_length; i++) {
        free(confs->hosts[i]);
        confs->hosts[i] = NULL;
    }
    free(confs->hosts);
    confs->hosts = NULL;
    free(confs);
    confs = NULL;
}

void print_conf_set(conf_set *confs) {
    printf("header_length: %d\n", confs->header_length);
    for(int i=0; i<confs->header_length; i++)
        printf("header[%d]: %s\n", i, confs->header[i]);
    printf("seperation_length: %d\n", confs->seperation_length);
    printf("%s\n", confs->seperation);
    printf("hosts_perpage: %d\n", confs->hosts_perpage);
    printf("pages: %d\n", confs->pages);
    printf("hosts_length: %d\n", confs->hosts_length);
    for(int i=0; i<confs->hosts_length; i++){
        printf("%-15.14s%-18.17s%-8.7s%-15.14s\n",
                confs->hosts[i]->hostname,
                confs->hosts[i]->ip,
                confs->hosts[i]->port,
                confs->hosts[i]->username);
    }
}
