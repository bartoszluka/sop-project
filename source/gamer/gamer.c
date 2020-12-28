#include "gamer.h"

Gamer *newGamer(int position)
{
    Gamer *gamer = (Gamer *)malloc(sizeof(Gamer));
    if (!gamer)
    {
        ERR("malloc");
    }
    gamer->position = position;
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        gamer->items[i] = NULL;
    }
    return gamer;
}
void freeGamer(Gamer *gamer)
{
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (gamer->items[i])
            free(gamer->items[i]);
    }
    free(gamer);
}

void printGamer(Gamer *gamer)
{
    if (!gamer)
    {
        printf("gamer is NULL\n");
        return;
    }
    printf("you are in room %d\n", gamer->position);
    printf("your items are:\n");
    printAllItems(gamer->items);
}

int gamerHasSlot(Gamer *gamer)
{
    return hasSlot(gamer->items);
}

void emptyGamersInv(Gamer *gamer){
    
}