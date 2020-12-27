#include "list.h"

Node *newNode(void *data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        ERR("malloc failed");
    }
    node->data = data;
    node->next = NULL;
    return node;
}

int listHasItem(List *list, int item)
{
    Node *p = list->head;
    while (p)
    {
        if (*(int *)(p->data) == item)
        {
            return 1;
        }
        p = p->next;
    }
    return 0;
}
void printIntNode(Node *node)
{
    printf("%d\n", *(int *)(node->data));
}
void printRouteNode(Node *node)
{
    printf("%d", *(int *)(node->data));
    if (node->next)
    {
        printf(" -> ");
    }
    else
    {
        printf("\n");
    }
}

void freeNode(Node *node)
{
    free(node->data);
    free(node);
}
void foreachNode(List *list, void (*doThis)(Node *))
{
    Node *p = list->head;
    while (p)
    {
        Node *tmp = p;
        p = p->next;
        doThis(tmp);
    }
}

void printRoute(List *list)
{
    foreachNode(list, printRouteNode);
}
void printIntList(List *list)
{
    if (!list || !list->head)
    {
        printf("list is NULL\n");
    }
    else
    {
        foreachNode(list, printIntNode);
    }
}

List *newList()
{
    List *list = NULL;
    list = (List *)malloc(sizeof(*list));
    if (!list)
    {
        ERR("malloc failed");
    }
    list->head = NULL;
    list->Count = 0;
    return list;
}

void freeList(List *list)
{
    foreachNode(list, freeNode);
    free(list);
}
void addToList(List *list, Node *node)
{
    if (!(list->head))
    {
        list->head = node;
    }
    else
    {
        node->next = list->head;
        list->head = node;
    }
    list->Count++;
}

void addIntItemToList(List *list, int value)
{
    int *data = (int *)malloc(sizeof(int));
    *data = value;
    Node *node = newNode(data);
    addToList(list, node);
}