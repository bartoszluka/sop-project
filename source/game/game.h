#ifndef GAME_H_
#define GAME_H_

#include "../room/room.h"
#include "../gamer/gamer.h"
#include "../item/item.h"
#include "../map/map.h"

void pickUpItem(Gamer *gamer, Room *room, int itemId);

int dropItem(Gamer *gamer, Room *room, int eqItemId);

void optionMoveTo(Room **rooms, Gamer *gamer, int optionInt);

void optionPickUp(Room **rooms, Gamer *gamer, int optionInt);

void optionDrop(Room **rooms, Gamer *gamer, int optionInt);

void optionFindPath(Room **rooms, int size, Gamer *gamer, int threadCount, int to);
#endif