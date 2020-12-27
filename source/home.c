#include "save.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#define MAX_PATH 100


void gamerStuff()
{

    Gamer *gamer = newGamer(0);
    gamer->items[0] = newItem(1, 2);
    gamer->items[1] = newItem(3, 4);
    printGamer(gamer);
    freeGamer(gamer);
}
int hasSlot(Item **items)
{
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (!items[i])
        {
            return i;
        }
    }
    return -1;
}

int gamerHasSlot(Gamer *gamer)
{
    return hasSlot(gamer->items);
}

int roomHasSlot(Room room)
{
    return hasSlot(room.items);
}

int findItemInItems(Item **items, int itemId)
{
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (items[i] && items[i]->itemId == itemId)
        {
            return i;
        }
    }
    // printf("there is no such item in this room\n");
    return -1;
}
void pickUpItem(Gamer *gamer, Room *room, int itemId)
{
    int slot = gamerHasSlot(gamer);
    if (slot == -1)
    {
        printf("Your equipment is full!\n");
    }
    else
    {
        gamer->items[slot] = room->items[itemId];
        room->items[itemId] = NULL;
    }
}

int dropItem(Gamer *gamer, Room *room, int eqItemId)
{
    int slot = roomHasSlot(*room);
    if (slot == -1)
    {
        printf("This room is full!\n");
    }
    else
    {
        room->items[slot] = gamer->items[eqItemId];
        gamer->items[eqItemId] = NULL;
    }
    return slot;
}

void optionMoveTo(Room **rooms, Gamer *gamer, int optionInt)
{
    if (listHasItem(rooms[gamer->position]->connectedRooms, optionInt))
    {
        gamer->position = optionInt;
        printf("you moved to room %d\n", optionInt);
    }
    else
    {
        printf("you can't go to this room\n");
        printf("rooms you can go to are:\n");
        printIntList(rooms[gamer->position]->connectedRooms);
    }
}

void optionPickUp(Room **rooms, Gamer *gamer, int optionInt)
{
    int slot = findItemInItems(rooms[gamer->position]->items, optionInt);
    if (slot != -1)
    {
        printf("you picked up the item\n");
        printItem(rooms[gamer->position]->items[slot]);
        pickUpItem(gamer, rooms[gamer->position], slot);
    }
    else
    {
        printf("you can't pick up this item\n");
        printf("items you can pick up are:\n");
        printAllItems(rooms[gamer->position]->items);
    }
}

void optionDrop(Room **rooms, Gamer *gamer, int optionInt)
{
    int slot = findItemInItems(gamer->items, optionInt);
    if (slot != -1)
    {
        printItem(gamer->items[slot]);
        if (dropItem(gamer, rooms[gamer->position], slot) != -1)
        {
            printf("you successfully dropped the item\n");
        }
    }
    else
    {
        printf("you don't have such item in your inventory\n");
    }
}

void optionFindPath(Room **rooms, int size, Gamer *gamer, int threadCount, int to)
{
    findRouteFromTo(gamer->position, to, rooms, size, threadCount);
}
void game()
{
    const char *path = "save3.save";
    // scanf("%d", &n);
    Room **rooms;
    Gamer *gamer;
    int roomsArraySize;
    readSaveFile(&rooms, &gamer, path, &roomsArraySize);

    if (!rooms)
    {
        ERR("bad rooms read");
    }
    if (!gamer)
    {
        ERR("bad gamer read");
    }
    puts("=====gamer=====");
    printGamer(gamer);
    puts("=====rooms=====");
    printRooms(rooms, roomsArraySize);

    // generateRandomMap(&rooms, n);
    // srand(time(NULL));
    // int startPosition = rand() % n;
    printf("welcome\n");
    printRoom(*rooms[gamer->position]);
    printf("\n");

    const char *moveto = "move-to";
    const char *drop = "drop";
    const char *save = "save";
    const char *quit = "quit";
    const char *pickup = "pick-up";
    const char *findpath = "find-path";
    const char *listme = "list-me";
    const char *listroom = "list-room";

    int optionInt;
    char option[20];
    while (1)
    {
        scanf("%s", option);

        if (strcmp("quit", option) == 0)
        {
            freeRoomsArray(rooms, roomsArraySize);
            // free(rooms);
            freeGamer(gamer);
            break;
        }
        else if (strcmp(moveto, option) == 0)
        {
            scanf("%d", &optionInt);
            optionMoveTo(rooms, gamer, optionInt);
        }
        else if (strcmp(pickup, option) == 0)
        {
            scanf("%d", &optionInt);
            optionPickUp(rooms, gamer, optionInt);
        }
        else if (strcmp(drop, option) == 0)
        {
            scanf("%d", &optionInt);
            optionDrop(rooms, gamer, optionInt);
        }
        else if (strcmp(findpath, option) == 0)
        {
            int optionInt2;
            scanf("%d", &optionInt);
            scanf("%d", &optionInt2);
            optionFindPath(rooms, roomsArraySize, gamer, optionInt, optionInt2);
        }
        else if (strcmp(save, option) == 0)
        {
            char savepath[MAX_PATH];
            scanf("%s", savepath);
            writeSaveFile(rooms, gamer, roomsArraySize, savepath);
        }
        else if (strcmp("list-me", option) == 0)
        {
            printGamer(gamer);
        }
        else if (strcmp("list-room", option) == 0)
        {
            printRoom(*rooms[gamer->position]);
        }
        else
        {
            printf("command \"%s\" not found, try one of these instead\n", option);
            printf("%s\n", moveto);
            printf("%s\n", drop);
            printf("%s\n", save);
            printf("%s\n", quit);
            printf("%s\n", pickup);
            printf("%s\n", findpath);
            printf("%s\n", listme);
            printf("%s\n", listroom);
        }
        printf("\n");
    }
}
int main(int argc, char *argv[])
{
    char c;
    char savepath[MAX_PATH] = "";
    while ((c = getopt(argc, argv, "b:")) != -1)
    {
        switch (c)
        {
        case 'b':
            // printf("%s\n", optarg);
            strcpy(savepath, optarg);
            break;
        case ':':
            fprintf(stderr,
                    "Option -%c requires an operand\n", optopt);
            break;
        case '?':
            fprintf(stderr,
                    "Unrecognized option: '-%c'\n", optopt);
            break;
        }
    }

    const char *autoSaveEnv = "GAME_AUTOSAVE";
    if (strlen(savepath) > 0)
    {
        printf("%s\n", savepath);
        char env[2 * MAX_PATH];
        sprintf(env, "%s=%s", autoSaveEnv, savepath);
        printf("%s\n", env);
        if (putenv(env))
        {
            ERR("putenv");
        }
        char *env2 = getenv(autoSaveEnv);
        printf("%s\n", env2);
    }
    //generateStuff();
    // saveStuff();
    // game();

    // gamerStuff();
    // free(rooms);
    return 0;
}