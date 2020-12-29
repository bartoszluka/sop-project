#ifndef ROOM_H_
#define ROOM_H_

#include "../list/list.h"
#include "../item/item.h"
#include "../err.h"

typedef struct Room
{
    int roomId;
    List *connectedRooms;
    Item *items[ITEMS_IN_ROOM];
} Room;

Room *newRoom(int roomId);

Room **createRooms(int size);

void freeRoom(Room *room);

void freeRoomsArray(Room *rooms[], int size);

void printRoom(Room *room);

void printRooms(Room *rooms[], int arraySize);

int roomIsEmpty(Room *room);

int roomsAreEmpty(Room *rooms[], int size);

int roomHasSlot(Room *room);

#endif //ROOM_H_