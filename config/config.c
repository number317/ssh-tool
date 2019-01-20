#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"

void error_handle(int condition, char *message) {
    if(condition) {
        fprintf(stderr, message);
        exit(1);
    }
}

config_t* set_config_file(config_t *config, char *config_file) {
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
char** get_header(
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
char* get_seperation(
        config_t *config,
        char *seperation,
        int seperation_length) {
    char *seperation_char = (char *)config_setting_get_string(
            config_lookup(config, "seperation_char"));
    error_handle(!seperation_char,
            "Error! Config file doesn't have the seperation_char config\n");
    /*
     * make sure seperation has the enough space
     */
    int char_width = strlen(seperation_char);
    char *new_seperation;
    new_seperation = realloc(seperation, seperation_length*char_width+1);
    error_handle(!new_seperation,
            "Error! Realloc memory error for seperation\n");
    memset(new_seperation, 0, seperation_length*char_width+1);
    for(int i=0; i<seperation_length;i++)
        snprintf(new_seperation+i*char_width, char_width+1, seperation_char);
    return new_seperation;
}

host** get_hosts(
        config_t *config,
        host **hosts,
        int hosts_length) {
    config_setting_t *host_list = config_lookup(config, "hosts");
    error_handle(!host_list,
            "Error! Config file doesn't have the hosts config\n");
    host **new_hosts;
    new_hosts = realloc(hosts, hosts_length*sizeof(host*));
    error_handle(!new_hosts,
            "Error! Realloc memory error for hosts\n");
    for(int i=0; i<hosts_length; i++) {
        host *temp = (host*)malloc(sizeof(host));
        config_setting_t *hosts_item = config_setting_get_elem(host_list, i);
        config_setting_lookup_string(hosts_item, "hostname", &(temp->hostname));
        config_setting_lookup_string(hosts_item, "ip", &(temp->ip));
        config_setting_lookup_string(hosts_item, "port", &(temp->port));
        config_setting_lookup_string(hosts_item, "username", &(temp->username));
        config_setting_lookup_string(hosts_item, "password", &(temp->password));
        config_setting_lookup_string(hosts_item, "comment", &(temp->comment));
        new_hosts[i]=temp;
    }
    return new_hosts;
}
