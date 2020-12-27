#ifndef ITEM_H_
#define ITEM_H_

#include "../err.h"
#include <stdlib.h>
#include <stdio.h>
#define ITEMS_IN_ROOM 2

typedef struct Item
{
    int itemId;
    int destinationRoomId;
} Item;

Item *newItem(int itemId, int destinationRoomId);

void printItem(Item *item);

void printAllItems(Item *items[ITEMS_IN_ROOM]);

void freeItem(Item *item);

int hasSlot(Item *items[ITEMS_IN_ROOM]);

int findItemInItems(Item **items, int itemId);

#endif //ITEM_H_