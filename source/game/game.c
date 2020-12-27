#include "game.h"

void pickUpItem(Gamer *gamer, Room *room, int itemId)
{
    int slot = gamerHasSlot(gamer);
    if (slot == -1)
    {
        printf("Your equipment is full!\n");
    }
    else
    {
        gamer->items[slot] = room->items[itemId];
        room->items[itemId] = NULL;
    }
}

int dropItem(Gamer *gamer, Room *room, int eqItemId)
{
    int slot = roomHasSlot(*room);
    if (slot == -1)
    {
        printf("This room is full!\n");
    }
    else
    {
        room->items[slot] = gamer->items[eqItemId];
        gamer->items[eqItemId] = NULL;
    }
    return slot;
}

void optionMoveTo(Room **rooms, Gamer *gamer, int optionInt)
{
    if (listHasItem(rooms[gamer->position]->connectedRooms, optionInt))
    {
        gamer->position = optionInt;
        printf("you moved to room %d\n", optionInt);
    }
    else
    {
        printf("you can't go to this room\n");
        printf("rooms you can go to are:\n");
        printIntList(rooms[gamer->position]->connectedRooms);
    }
}

void optionPickUp(Room **rooms, Gamer *gamer, int optionInt)
{
    int slot = findItemInItems(rooms[gamer->position]->items, optionInt);
    if (slot != -1)
    {
        printf("you picked up the item\n");
        printItem(rooms[gamer->position]->items[slot]);
        pickUpItem(gamer, rooms[gamer->position], slot);
    }
    else
    {
        printf("you can't pick up this item\n");
        printf("items you can pick up are:\n");
        printAllItems(rooms[gamer->position]->items);
    }
}

void optionDrop(Room **rooms, Gamer *gamer, int optionInt)
{
    int slot = findItemInItems(gamer->items, optionInt);
    if (slot != -1)
    {
        printItem(gamer->items[slot]);
        if (dropItem(gamer, rooms[gamer->position], slot) != -1)
        {
            printf("you successfully dropped the item\n");
        }
    }
    else
    {
        printf("you don't have such item in your inventory\n");
    }
}

void optionFindPath(Room **rooms, int size, Gamer *gamer, int threadCount, int to)
{
    findRouteFromTo(gamer->position, to, rooms, size, threadCount);
}