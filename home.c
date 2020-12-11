#include "list.h"

int main(int argc, char *argv[])
{
    List *list = newList();

    addIntsToList(list, 7, 7);
    addIntsToList(list, 3, 3);
    addIntsToList(list, 1, 1);
    addIntsToList(list, 2, 2);

    printList(list);

    freeList(list);

    return 0;
}