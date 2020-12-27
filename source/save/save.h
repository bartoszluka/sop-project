#ifndef SAVE_H_
#define SAVE_H_

#include "../room/room.h"
#include "../item/item.h"
#include "../gamer/gamer.h"
#include "../err.h"

void readItemsFromFile(FILE *infile, Item *items[ITEMS_IN_ROOM]);

void readSaveFile(Room ***roomsPtr, Gamer **gamerPtr, const char *path, int *sizeOfArray);

void writeItemsToFile(FILE *outfile, Item *items[ITEMS_IN_ROOM]);

void writeSaveFile(Room **rooms, Gamer *gamer, int size, const char *path);

#endif //SAVE_H_