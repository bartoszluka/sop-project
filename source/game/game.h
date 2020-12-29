#ifndef GAME_H_
#define GAME_H_

#include "../room/room.h"
#include "../gamer/gamer.h"
#include "../item/item.h"
#include "../map/map.h"
#include "../save/save.h"
#include "../err.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PATH 100

void pickUpItem(Gamer *gamer, Room *room, int itemId);

int dropItem(Gamer *gamer, Room *room, int eqItemId);

void optionMoveTo(Room **rooms, Gamer *gamer, int optionInt);

void optionPickUp(Room **rooms, Gamer *gamer, int optionInt);

void optionDrop(Room **rooms, Gamer *gamer, int optionInt);

void optionFindPath(Room **rooms, int size, Gamer *gamer, int threadCount, int to);

void startNewGame(const char *path);

void loadGame(const char *path);

void playGame(Room **rooms, int size, Gamer *gamer);

void parseInput(int argc, char *argv[]);

void mainMenu();

void setEnvVariable(char *savepath);

#endif //GAME_H_