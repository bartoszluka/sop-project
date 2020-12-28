#include "game/game.h"

// void test()
// {
//     int size;
//     const char *path = "save2.save";
//     // scanf("%d", &n);
//     Room **rooms = NULL;
//     readSaveFile(&rooms, path, &size);

//     if (!rooms)
//     {
//         ERR("bad read");
//     }
//     //generateRandomMap(&rooms, size);

//     freeRoomsArray(rooms, size);
// }

void test2()
{
    List *list = newList();
    char line[] = " 0 1 2 3";
    parseLine(line, list);
    printIntList(list);
    freeList(list);
}

void readTest()
{
    Room **rooms = NULL;
    Gamer *gamer = NULL;
    int sizeOfArray;
    const char *path = "save3.save";
    readSaveFile(&rooms, &gamer, path, &sizeOfArray);

    puts("=====gamer=====");
    printGamer(gamer);
    puts("=====rooms=====");
    printRooms(rooms, sizeOfArray);

    freeRoomsArray(rooms, sizeOfArray);
    freeGamer(gamer);
}

int main(int argc, char *argv[])
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int i = 0;

    pthread_mutex_lock(&mutex);
    i++;
    pthread_mutex_unlock(&mutex);
    return 0;
}