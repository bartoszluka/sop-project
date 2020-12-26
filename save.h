#include "list.h"
#include <string.h>
#define ITEMS_IN_ROOM 2
#define MAX_STRING_LENGTH 1024

typedef struct Item
{
    int itemId;
    int destinationRoomId;
} Item;

typedef struct Gamer
{
    int position;
    Item *items[ITEMS_IN_ROOM];
} Gamer;

typedef struct Room
{
    int roomId;
    List *connectedRooms;
    Item *items[ITEMS_IN_ROOM];
} Room;

Gamer *newGamer(int position)
{
    Gamer *gamer = (Gamer *)malloc(sizeof(Gamer));
    if (!gamer)
    {
        ERR("malloc");
    }
    gamer->position = position;
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        gamer->items[i] = NULL;
    }
    return gamer;
}
void freeGamer(Gamer *gamer)
{
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (gamer->items[i])
            free(gamer->items[i]);
    }
    free(gamer);
}

Item *newItem(int itemId, int destinationRoomId)
{
    Item *item = (Item *)malloc(sizeof(Item));
    if (!item)
    {
        ERR("malloc");
    }
    item->itemId = itemId;
    item->destinationRoomId = destinationRoomId;
    return item;
}

Room *newRoom(int roomId)
{
    Room *room = (Room *)malloc(sizeof(Room));
    if (!room)
    {
        ERR("malloc");
    }

    room->roomId = roomId;
    room->connectedRooms = newList();

    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        room->items[i] = NULL;
    }

    return room;
}

Room **createRooms(int size)
{
    Room **rooms = (Room **)malloc(sizeof(*rooms) * size);
    if (!rooms)
    {
        ERR("malloc");
    }
    for (int i = 0; i < size; i++)
    {
        rooms[i] = newRoom(i);
    }
    return rooms;
}
void printItem(Item *item)
{
    if (item)
    {
        printf("id: %d ", item->itemId);
        printf("dest: %d\n", item->destinationRoomId);
    }
    else
    {
        // printf("item is NULL\n");
    }
}

void printAllItems(Item **items)
{
    int count = 0;
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (items[i])
        {
            count++;
        }
        printItem(items[i]);
    }
    if (count == 0)
    {
        printf("you have no items\n");
    }
}
void printGamer(Gamer *gamer)
{
    if (!gamer)
    {
        printf("gamer is NULL\n");
        return;
    }
    printf("you are in room %d\n", gamer->position);
    printf("your items are:\n");
    printAllItems(gamer->items);
}

void freeItem(Item *item)
{
    free(item);
}
void freeRoom(Room *room)
{
    freeList(room->connectedRooms);

    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (room->items[i])
            freeItem(room->items[i]);
    }
    // z tym jest "double free or corruption (out)"
    free(room);
}

void freeRoomsArray(Room **rooms, int size)
{
    // free(rooms);
    for (int i = 0; i < size; i++)
    {
        if (rooms[i])
            freeRoom(rooms[i]);
    }
    // z tym jest "double free or corruption (out)"
    free(rooms);
}

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

void parseLine(char *line, List *list)
{
    char *saveptr;
    char *token = strtok_r(line, " ", &saveptr);
    while (token != NULL)
    {
        int roomNo = atoi(token);
        addIntsToList2(list, roomNo);
        token = strtok_r(NULL, " ", &saveptr);
    }
}

void printRoom(Room room)
{
    printf("room number %d:\n", room.roomId);
    printf("list: \n");
    printIntList(room.connectedRooms);
    printf("items: \n");
    for (int j = 0; j < ITEMS_IN_ROOM; j++)
    {
        printItem(room.items[j]);
    }
}

void printRooms(Room *rooms[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        printRoom(*rooms[i]);
    }
}

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
            addIntsToList2(rooms[roomNo]->connectedRooms, roomId);
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