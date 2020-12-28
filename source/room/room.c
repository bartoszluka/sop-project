#include "room.h"

Room *newRoom(int roomId)
{
    Room *room = (Room *)malloc(sizeof(Room));
    if (!room)
    {
        ERR("malloc");
    }

    room->roomId = roomId;
    room->connectedRooms = newList();

    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        room->items[i] = NULL;
    }

    return room;
}

Room **createRooms(int size)
{
    Room **rooms = (Room **)malloc(sizeof(*rooms) * size);
    if (!rooms)
    {
        ERR("malloc");
    }
    for (int i = 0; i < size; i++)
    {
        rooms[i] = newRoom(i);
    }
    return rooms;
}

void freeRoom(Room *room)
{
    freeList(room->connectedRooms);

    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (room->items[i])
        {
            freeItem(room->items[i]);
        }
    }
    free(room);
}

void freeRoomsArray(Room **rooms, int size)
{
    // free(rooms);
    for (int i = 0; i < size; i++)
    {
        if (rooms[i])
            freeRoom(rooms[i]);
    }
    free(rooms);
}
void printRoom(Room *room)
{
    printf("room number %d:\n", room->roomId);
    printf("list: \n");
    printIntList(room->connectedRooms);
    printf("items: \n");
    for (int j = 0; j < ITEMS_IN_ROOM; j++)
    {
        printItem(room->items[j]);
    }
}

void printRooms(Room *rooms[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        printRoom(rooms[i]);
    }
}

int roomsAreEmpty(Room *rooms[], int size){
    for (int i = 0; i < size; i++)
    {
        if (roomHasSlot(rooms[i]) > -1)
        {
            return 0;
        }
    }
    return 1;
}

int roomHasSlot(Room *room)
{
    return hasSlot(room->items);
}
