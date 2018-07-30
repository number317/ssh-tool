#include<stdio.h>

typedef struct host{
    char* group;
    char* hostname;
    char* ip;
    char* port;
    char* user;
    char* password;
    char* intro;
}host;

void printHost(host*);
host* initHost(host* host, char*, char*, char*, char*, char*, char*, char*);
