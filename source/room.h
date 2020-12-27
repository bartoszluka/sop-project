#include "list.h"
#include "item.h"
#include <string.h>
#define ITEMS_IN_ROOM 2

// #define MAX_STRING_LENGTH 1024

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