#include "save.h"
#include <sys/stat.h>
#include <sys/types.h>
void saveStuff();

void printRooms(Room *rooms[], int n)
{
    for (int i = 0; i < n; i++)
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

int main(int argc, char *argv[])
{
    saveStuff();

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
