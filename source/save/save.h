#ifndef SAVE_H_
#define SAVE_H_

#include "../room/room.h"
#include "../item/item.h"
#include "../gamer/gamer.h"
#include "../err.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define AUTOSAVE_INTERVAL 10
#define SAVED_BY_USER 2137
#define AUTO_SAVED 2138

void readItemsFromFile(FILE *infile, Item *items[ITEMS_IN_ROOM]);

void readSaveFile(Room ***roomsPtr, Gamer **gamerPtr, const char *path, int *sizeOfArray);

void writeItemsToFile(FILE *outfile, Item *items[ITEMS_IN_ROOM]);

void writeSaveFile(Room **rooms, Gamer *gamer, int size, const char *path);

typedef struct autoSaveArgs
{
    Room ***rooms;
    Gamer **gamer;
    int size;
    const char *path;
    sigset_t *pMask;
    int *saved;
} autoSaveArgs;

void doNothing(int signal);

void setLastSignal(int signal);

void sethandler(void (*f)(int), int sigNo);

void *autoSave(void *args);

#endif //SAVE_H_