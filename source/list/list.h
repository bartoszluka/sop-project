#ifndef LIST_H_
#define LIST_H_

#include "../err.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    int Count;
} List;

Node *newNode(int data);

int listHasItem(List *list, int item);

void printIntNode(Node *node);

void printRouteNode(Node *node);

void freeNode(Node *node);

void foreachNode(List *list, void (*doThis)(Node *));

void printRoute(List *list);

void printIntList(List *list);

List *newList();

void freeList(List *list);

void addToList(List *list, Node *node);

void addIntItemToList(List *list, int value);

void copyList(List *from, List *to);

Node *removeFirstItemFromList(List *list);


#endif //LIST_H