#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "host.h"
#include "node.h"

int main(int argc, char* argv[]){
    host *h1=(host*)malloc(sizeof(host)), *h2=(host*)malloc(sizeof(host));

    initHost(h1,"", "idc-dev", "10.0.52.184", "22", "appuser", "Hello@05092018", "idc dev & uat env");
    initHost(h2,"", "idc-proxy", "10.0.6.44", "22", "appuser", "Hello0430@2018", "idc prod proxy");

    node *hostList=(node*)malloc(sizeof(node));
    node *temp=(node*)malloc(sizeof(node));
    temp->data=h2;
    temp->next=NULL;
    hostList->data=h1;
    hostList->next=temp;
    printHost((host*)hostList->data);
    printf("lenth of hostList: %d\n", lenOfList(hostList));
    printNode(hostList);
    printHost(indexOfList(hostList,1)->data);
    printHost(hostList->data);
    printHost(hostList->next->data);
    return 0;
}
