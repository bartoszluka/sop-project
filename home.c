#include "save.h"
#include <sys/stat.h>
#include <sys/types.h>

void freeMatrix(List *vertices[], int n);

typedef struct Room
{
    int roomId;
    List *coonectedRooms;
    List *items;
} Room;

typedef struct Item
{
    int itemId;
    int destinationRoomId;
} Item;

int main(int argc, char *argv[])
{

    const char *path = "save2.save";
    int n = 5;
    // scanf("%d", &n);
    List **vertices;
    vertices = readSaveFile(path, &n);

    if (!vertices)
    {
        ERR("bad read");
    }

    const char *path2 = "save.save";
    writeSaveFile(vertices, n, path2);
    // saveMatrixToFile(vertices, n, path);
    // readFromFile("save2");
    freeMatrix(vertices, n);

    return 0;
}
void freeMatrix(List *vertices[], int n)
{
    for (int i = 0; i < n; i++)
    {
        freeList(vertices[i]);
    }
    free(vertices);
}