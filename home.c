#include "save.h"
#include <sys/stat.h>
#include <sys/types.h>
void saveStuff();

void printRooms(Room *rooms[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        printf("room number %d:\n", i);
        printf("list: \n");
        printIntList(rooms[i]->connectedRooms);
        printf("items: \n");
        for (int j = 0; j < ITEMS_IN_ROOM; j++)
        {
            printItem(rooms[i]->items[j]);
        }
    }
}

void generateRandomMap(Room **roomsPtr, int arraySize)
{
    Room **rooms = roomsPtr;
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
}

void generateStuff()
{
    int arraySize = 5;
    scanf("%d", &arraySize);
    Room **rooms = createRooms(arraySize);

    generateRandomMap(rooms, arraySize);
    printRooms(rooms, arraySize);
    freeRoomsArray(rooms, arraySize);
}

int main(int argc, char *argv[])
{
    generateStuff();

    // free(rooms);
    return 0;
}

void saveStuff()
{

    int n;
    const char *path = "save2.save";
    // scanf("%d", &n);
    Room **vertices;
    vertices = readSaveFile(path, &n);

    if (!vertices)
    {
        ERR("bad read");
    }

    printRooms(vertices, n);

    // free(vertices);
    const char *path2 = "save.save";
    writeSaveFile(vertices, n, path2);

    freeRoomsArray(vertices, n);
}
