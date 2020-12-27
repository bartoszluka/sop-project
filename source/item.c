#include "item.h"

Item *newItem(int itemId, int destinationRoomId)
{
    Item *item = (Item *)malloc(sizeof(Item));
    if (!item)
    {
        ERR("malloc");
    }
    item->itemId = itemId;
    item->destinationRoomId = destinationRoomId;
    return item;
}

void printItem(Item *item)
{
    if (item)
    {
        printf("id: %d ", item->itemId);
        printf("dest: %d\n", item->destinationRoomId);
    }
}
void printAllItems(Item *items[ITEMS_IN_ROOM])
{
    int count = 0;
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (items[i])
        {
            count++;
        }
        printItem(items[i]);
    }
    if (count == 0)
    {
        printf("you have no items\n");
    }
}

void freeItem(Item *item)
{
    free(item);
}