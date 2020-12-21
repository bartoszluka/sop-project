#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <string.h>

#define ERR(source) (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), \
                     perror(source), kill(0, SIGKILL),               \
                     exit(EXIT_FAILURE))

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

Node *newNode(void *data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        ERR("malloc failed");
    }
    // node->itemId = itemId;
    // node->destRoomId = destRoomId;
    node->data = data;
    node->next = NULL;
    return node;
}

typedef struct List
{
    Node *head;
    int Count;
} List;

void printNode(Node *node)
{
    // printf("item id = %d\n", node->itemId);
    // printf("destination id = %d\n", node->destRoomId);
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

void printList(List *list)
{
    foreachNode(list, printNode);
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
        // printf("count is %d\n", list->Count);
        foreachNode(list, printIntNode);
    }
}

List *newList()
{
    List *list = (List *)malloc(sizeof(List));
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

void addIntsToList(List *list, int itemId, int destRoomId)
{
    int *data = (int *)malloc(sizeof(int));
    Node *node = newNode(data);
    addToList(list, node);
}
void addIntsToList2(List *list, int value)
{
    int *data = (int *)malloc(sizeof(int));
    *data = value;
    Node *node = newNode(data);
    addToList(list, node);
}