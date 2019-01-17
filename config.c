#include <string.h>
#include <stdlib.h>
#include "config.h"

/*
 * header is an array used to save table's header
 * header_length is the length of array header
 */
char** get_header(
        config_t *config,
        char **header,
        int header_length
        ) {
    config_setting_t *headers = config_lookup(config, "header");
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
        int seperation_length
        ) {
    char *seperation_char = (char *)config_setting_get_string(
            config_lookup(config, "seperation_char")
            );
    /*
     * make sure seperation has the enough space
     */
    int char_width = strlen(seperation_char);
    if(!realloc(seperation,seperation_length*(char_width+1))) {
        printf("error occur when realloc for seperation\n");
        exit(1);
    }
    memset(seperation, 0, seperation_length*(strlen(seperation_char)+1));
    for(int i=0; i<seperation_length;i++)
        sprintf(seperation+i*char_width, seperation_char);
    return seperation;
}

host** get_hosts(
        config_t *config,
        host **hosts,
        int hosts_length
        ) {
    config_setting_t *hosts_array = config_lookup(config, "hosts");
    for(int i=0; i<hosts_length; i++) {
        host *temp = (host*)malloc(sizeof(host));
        config_setting_t *hosts_item = config_setting_get_elem(hosts_array, i);
        config_setting_lookup_string(hosts_item, "hostname", &(temp->hostname));
        config_setting_lookup_string(hosts_item, "ip", &(temp->ip));
        config_setting_lookup_string(hosts_item, "port", &(temp->port));
        config_setting_lookup_string(hosts_item, "username", &(temp->username));
        config_setting_lookup_string(hosts_item, "password", &(temp->password));
        config_setting_lookup_string(hosts_item, "comment", &(temp->comment));
        hosts[i]=temp;
    }
    return hosts;
}
