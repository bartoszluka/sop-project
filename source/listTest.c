#include "game/game.h"

void addConnection(Room **rooms, int room1, int room2)
{
    addIntItemToList(rooms[room1]->connectedRooms, room2);
    addIntItemToList(rooms[room2]->connectedRooms, room1);
}

void generateConnections(Room **rooms, int arraySize)
{
    unsigned int pid = getpid();
    int rand = rand_r(&pid) % (arraySize);
    //generating random permutation of n numbers
    int *permutation = (int *)calloc(sizeof(int), arraySize);
    for (int i = 0; i < arraySize; i++)
    {
        while (permutation[rand] != 0)
        {
            rand = rand_r(&pid) % (arraySize);
        }
        permutation[rand] = i;
    }

    //adding connection between next rooms in permutation array
    for (int i = 0; i < arraySize - 1; i++)
    {
        addConnection(rooms, permutation[i], permutation[i + 1]);
    }
    free(permutation);

    //adding some more random connections
    for (int i = 0; i < arraySize; i++)
    {
        int room1 = rand_r(&pid) % (arraySize);
        int room2 = rand_r(&pid) % (arraySize);

        while (room1 == room2 || listHasItem(rooms[room1]->connectedRooms, room2))
        {
            room2 = rand_r(&pid) % (arraySize);
        }
        addConnection(rooms, room1, room2);
    }
}

void generateRandomMap(Room ***roomsPtr, int arraySize)
{
    Room **rooms = createRooms(arraySize);

    generateConnections(rooms, arraySize);

    int numberOfItems = 3 * arraySize / 2;
    srand(time(NULL));
    unsigned int pid = getpid();
    int rand = rand_r(&pid) % (arraySize);

    int *destinationRooms = (int *)calloc(sizeof(int), arraySize);

    for (int i = 0; i < numberOfItems; i++)
    {
        int roomId = i % arraySize;
        if (!rooms[roomId])
        {
            rooms[roomId] = newRoom(roomId);
        }

        int j = 0;
        //czy w pokoj o numerze j jest pelny
        while (j < ITEMS_IN_ROOM && rooms[roomId]->items[j])
        {
            j++;
        }
        //jesli nie to dodajemy mu przedmiot
        if (j < ITEMS_IN_ROOM)
        {
            while (destinationRooms[rand] >= 2 || rand == roomId)
            {
                rand++;
                rand = rand % arraySize;
                // rand = rand_r(getpid()) % (arraySize);
            }
            rooms[roomId]->items[j] = newItem(i, rand);
            destinationRooms[rand]++;
        }
        //jesli tak to idziemy dalej
    }
    free(destinationRooms);

    *roomsPtr = rooms;
}

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
    int n = 15;
    Room **rooms;
    generateRandomMap(&rooms, n);
    printRooms(rooms, n);
    freeRoomsArray(rooms, n);
    // test();
    return 0;
}