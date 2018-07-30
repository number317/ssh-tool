#include <stdio.h>
#include "node.h"

int lenOfList(node* start){
    int len=0;
    while(start!=NULL){
        len++;
        start=start->next;
    }
    return len;
}

node* indexOfList(node* start, int index){
    if(index>=lenOfList(start))
        return NULL;
    else
        for(int i=0; i<index; i++)
            start=start->next;
    return start;
}

void printNode(node* start){
    while(start!=NULL){
        printf("{%p, %p}\n", start->data, start->next);
        start=start->next;
    }
}
