#include <stdio.h>

typedef struct node{
    void* data;
    struct node* next;
}node;

int lenOfList(node*);

node* indexOfList(node*, int);

void printNode(node*);
