#include "game.h"

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
    int slot = roomHasSlot(room);
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

void playGame(Room **rooms, int roomsArraySize, Gamer *gamer)
{
    int saved = AUTO_SAVED;
    char *autosavePath = getenv("GAME_AUTOSAVE");
    autoSaveArgs saveArgs = {
        .rooms = &rooms,
        .gamer = &gamer,
        .size = roomsArraySize,
        .path = autosavePath,
        .saved = &saved,
    };

    sethandler(doNothing, SIGUSR2);

    pthread_t autosaveTid;
    pthread_create(&autosaveTid, NULL, autoSave, &saveArgs);
    // puts("=====gamer=====");
    // printGamer(gamer);
    // puts("=====rooms=====");
    // printRooms(rooms, roomsArraySize);

    // generateRandomMap(&rooms, n);
    // srand(time(NULL));
    // int startPosition = rand() % n;
    printf("welcome\n");
    printRoom(rooms[gamer->position]);
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
            *(saveArgs.saved) = AUTO_SAVED;
            pthread_kill(autosaveTid, SIGUSR2);
            pthread_join(autosaveTid, NULL);
            freeRoomsArray(rooms, roomsArraySize);
            freeGamer(gamer);
            puts("you quited the game");
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

            *(saveArgs.saved) = SAVED_BY_USER;
            pthread_kill(autosaveTid, SIGUSR2);
            puts("saving...");
            writeSaveFile(rooms, gamer, roomsArraySize, savepath);
            puts("saved");
        }
        else if (strcmp(listme, option) == 0)
        {
            printGamer(gamer);
        }
        else if (strcmp(listroom, option) == 0)
        {
            printRoom(rooms[gamer->position]);
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

void startNewGame(const char *path)
{
    Room **rooms;
    Gamer *gamer;
    int size;

    readSaveFile(&rooms, &gamer, path, &size);

    if (roomsAreEmpty(rooms, size))
    {
        addItemsToRooms(rooms, size);
    }

    srand(getpid() * size);

    gamer->position = rand() % size;
    emptyGamersInv(gamer);

    if (!rooms)
    {
        ERR("bad rooms read");
    }
    if (!gamer)
    {
        ERR("bad gamer read");
    }
    playGame(rooms, size, gamer);
}

void loadGame(const char *path)
{
    Room **rooms;
    Gamer *gamer;
    int size;

    readSaveFile(&rooms, &gamer, path, &size);

    if (!rooms)
    {
        ERR("bad rooms read");
    }
    if (!gamer)
    {
        ERR("bad gamer read");
    }
    playGame(rooms, size, gamer);
}

void setEnvVariable(char *savepath)
{
    const char *autoSaveEnv = "GAME_AUTOSAVE";
    if (strlen(savepath) == 0)
    {
        sprintf(savepath, "%s/%s", getenv("HOME"), ".game-autosave");
    }

    if (setenv(autoSaveEnv, savepath, 1))
    {
        ERR("setenv");
    }
}

void mainMenu()
{
    char option[20];
    while (1)
    {
        scanf("%s", option);

        if (strcmp("map-from-dir-tree", option) == 0)
        {
            char pathFrom[MAX_PATH];
            char pathTo[MAX_PATH];

            scanf("%s %s", pathFrom, pathTo);
            // scanf("%s", pathTo);
            mapFromDirTree(pathFrom, pathTo);
        }
        else if (strcmp("generate-random-map", option) == 0)
        {
            int n;
            char savepath[MAX_PATH];
            scanf("%d %s", &n, savepath);
            generateRandomMap(n, savepath);
        }
        else if (strcmp("start-game", option) == 0)
        {
            char pathFrom[MAX_PATH];
            scanf("%s", pathFrom);
            startNewGame(pathFrom);
        }
        else if (strcmp("load-game", option) == 0)
        {
            char pathFrom[MAX_PATH];
            scanf("%s", pathFrom);
            loadGame(pathFrom);
        }
        else if (strcmp("exit", option) == 0)
        {
            break;
        }
        else
        {
            printf("unknown command \"%s\"\n", option);
        }
    }
}

void parseInput(int argc, char *argv[])
{
    char c;
    char savepath[MAX_PATH] = "";
    while ((c = getopt(argc, argv, ":b:")) != -1)
    {
        switch (c)
        {
        case 'b':
            if (optarg)
            {
                strcpy(savepath, optarg);
            }
            break;
        case ':':
            switch (optopt)
            {
            case 'b':
                // strcpy(savepath, optarg);
                break;
            default:
                fprintf(stderr,
                        "Option -%c requires an operand\n", optopt);
                exit(EXIT_FAILURE);
            }
            break;
        case '?':
            fprintf(stderr,
                    "Unrecognized option: '-%c'\n", optopt);
            exit(EXIT_FAILURE);
            break;
        }
    }

    setEnvVariable(savepath);
}
