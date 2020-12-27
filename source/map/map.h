#ifndef MAP_H_
#define MAP_H_
#include "../room/room.h"
#include "../item/item.h"

#define MAX_NUMBER_OF_ITERATIONS 1000

typedef struct routeArguments
{
    Room **rooms;
    int u;
    int v;
    unsigned int seed;
} routeArguments;

typedef struct routeResult
{
    List *route;
    int exists;
} routeResult;

void addConnection(Room **rooms, int room1, int room2);

void generateConnections(Room **rooms, int arraySize);

void generateRandomMap(Room ***roomsPtr, int arraySize);

void *findRoute(void *args);

void findRouteFromTo(int from, int to, Room **rooms, int size, int threadCount);

#endif // MAP_H_