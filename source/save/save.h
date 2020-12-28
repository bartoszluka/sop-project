#ifndef SAVE_H_
#define SAVE_H_

#include "../room/room.h"
#include "../item/item.h"
#include "../gamer/gamer.h"
#include "../err.h"
#include <pthread.h>
#include <unistd.h>

#define AUTOSAVE_INTERVAL 5
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
    int *work;
} autoSaveArgs;

void sethandler(void (*f)(int), int sigNo);

void *autoSave(void *args);

#endif //SAVE_H_