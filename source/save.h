#include "item.h"
#include "room.h"

void readItemsFromFile(FILE *infile, Item *items[ITEMS_IN_ROOM])
{
    int itemId = -1, destinationId = -1;
    for (int j = 0; j < ITEMS_IN_ROOM; j++)
    {
        fscanf(infile, "(%d,%d)", &itemId, &destinationId);
        if (itemId > -1 && destinationId > -1)
        {
            items[j] = newItem(itemId, destinationId);
        }
        else
        {
            items[j] = NULL;
        }
    }
}
void readSaveFile(Room ***roomsPtr, Gamer **gamerPtr, const char *path, int *sizeOfArray)
{
    FILE *infile;
    infile = fopen(path, "r");
    if (infile == NULL)
    {
        ERR("fopen");
    }
    int roomNo, roomId, arraySize;
    fscanf(infile, "%d", &arraySize);

    Room **rooms = createRooms(arraySize);

    for (int i = 0; i < arraySize; i++)
    {
        fscanf(infile, "%d:", &roomNo);
        readItemsFromFile(infile, rooms[roomNo]->items);

        while (fscanf(infile, ", %d", &roomId) > 0)
        {
            addIntItemToList(rooms[roomNo]->connectedRooms, roomId);
        }
    }
    int gamerPosition;
    fscanf(infile, "%d::", &gamerPosition);
    Gamer *gamer = newGamer(gamerPosition);
    readItemsFromFile(infile, gamer->items);

    fclose(infile);
    *roomsPtr = rooms;
    *gamerPtr = gamer;
    *sizeOfArray = arraySize;
}

void writeItemsToFile(FILE *outfile, Item *items[ITEMS_IN_ROOM])
{
    for (int j = 0; j < ITEMS_IN_ROOM; j++)
    {
        if (items[j])
        {
            fprintf(outfile, "(%d,%d)", items[j]->itemId, items[j]->destinationRoomId);
        }
        else
        {
            fprintf(outfile, "(%d,%d)", -1, -1);
        }
    }
}

void writeSaveFile(Room **rooms, Gamer *gamer, int size, const char *path)
{
    FILE *outfile;

    outfile = fopen(path, "w");
    if (outfile == NULL)
    {
        ERR("fopen");
    }

    fprintf(outfile, "%d\n", size);
    for (int i = 0; i < size; i++)
    {

        fprintf(outfile, "%d:", i);

        writeItemsToFile(outfile, rooms[i]->items);

        Node *p = rooms[i]->connectedRooms->head;
        while (p)
        {
            Node *tmp = p;
            p = p->next;
            fprintf(outfile, ", %d", *(int *)(tmp->data));
        }
        fprintf(outfile, "\n");
    }

    fprintf(outfile, "%d::", gamer->position);
    writeItemsToFile(outfile, gamer->items);

    // close file
    fclose(outfile);
}