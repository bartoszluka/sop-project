#define _XOPEN_SOURCE 500
#ifndef MAP_H_
#define MAP_H_
#define MAXFD 20
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "../room/room.h"
#include "../item/item.h"
#include "../err.h"
#include "../save/save.h"
#include <pthread.h>
#include <ftw.h>
#include <dirent.h>
#include <string.h>

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

void generateRandomMap(int arraySize, const char *path);

void startNewGame(const char *path);

void *findRoute(void *args);

void findRouteFromTo(int from, int to, Room **rooms, int size, int threadCount);

void mapFromDirTree(const char *pathFrom, const char *pathTo);

int countFolders(const char *name, const struct stat *s, int type, struct FTW *f);

int addRoomsFromFolders(const char *name, const struct stat *s, int type, struct FTW *f);

#endif // MAP_H_