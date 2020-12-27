#ifndef GAMER_H_
#define GAMER_H_

#include "../err.h"
#include "../item/item.h"

typedef struct Gamer
{
    int position;
    Item *items[ITEMS_IN_ROOM];
} Gamer;

Gamer *newGamer(int position);

void freeGamer(Gamer *gamer);

void printGamer(Gamer *gamer);

int gamerHasSlot(Gamer *gamer);

#endif // GAMER_H_