#include "../game/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PATH 100

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

    int saved = AUTO_SAVED;
    char *autosavePath = getenv("GAME_AUTOSAVE");
    // puts(autosavePath);
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
            // kill(0, SIGUSR2);
            pthread_join(autosaveTid, NULL);
            freeRoomsArray(rooms, roomsArraySize);
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

void setEnvVariable(char *savepath)
{
    const char *autoSaveEnv = "GAME_AUTOSAVE";
    printf("%ld\n", strlen(savepath));
    if (strlen(savepath) == 0)
    {
        sprintf(savepath, "%s/%s", getenv("HOME"), ".game-autosave");
    }
    if (setenv(autoSaveEnv, savepath, 1))
    {
        ERR("setenv");
    }
    // puts(getenv(autoSaveEnv));
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
            generateRandomMap()
        }
        else if (strcmp("start-game", option) == 0)
        {
            break;
        }
        else if (strcmp("load-game", option) == 0)
        {
            break;
        }
        else if (strcmp("exit", option) == 0)
        {
            break;
        }
        else if (strcmp("exit", option) == 0)
        {
            break;
        }
        else
        {
            printf("unknown command \"%s\"", option);
        }
    }
}

int main(int argc, char *argv[])
{
    // mapFromDirTree("/etc", "newsave.txt");
    // return 0;
    // game();

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

    mainMenu();

    // game();

    return 0;
}