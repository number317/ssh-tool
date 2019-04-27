#include <stdio.h>
#include <stdlib.h>
int max(int a, int b){
    return a>b?a:b;
}

int min(int a, int b){
    return a<b?a:b;
}

void error_handle(int condition, char *message) {
    if(condition) {
        fprintf(stderr, "%s", message);
        exit(1);
    }
}
