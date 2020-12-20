#include "list.h"
#include <string.h>
#define ITEMS_IN_ROOM 2

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

Room **createRooms(int size)
{
    Room **rooms = (Room **)malloc(sizeof(Room *) * size);
    if (!rooms)
    {
        ERR("malloc");
    }
    return rooms;
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

void printItem(Item *item)
{
    if (item)
    {
        printf("id: %d ", item->itemId);
        printf("dest: %d\n", item->destinationRoomId);
    }
    else
    {
        printf("item is NULL\n");
    }
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
    // free(room);
}

void freeRoomsArray(Room **rooms, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (rooms[i])
            freeRoom(rooms[i]);
    }
    // z tym jest "double free or corruption (out)"
    // free(rooms);
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

Room **readSaveFile(const char *path, int *sizeOfArray)
{
    int filesize = fsize(path);
    char *buff = (char *)malloc(sizeof(char) * filesize);
    if (!buff)
        ERR("malloc");

    int fd;
    if ((fd = open(path, O_RDONLY)) < 0)
        ERR("open");

    //zmienic na bulk_read/obrone przed sygnalami
    if (read(fd, buff, filesize) < 0)
    {
        ERR("read");
    }

    char *saveptr;
    char *token = strtok_r(buff, "\n", &saveptr);

    *sizeOfArray = atoi(token);
    int number = 0;
    Room **rooms = createRooms(number);

    token = strtok_r(NULL, ":", &saveptr);
    while (token != NULL)
    {

        number = atoi(token);
        rooms[number] = newRoom(number);

        char *token2 = strtok_r(NULL, "\n", &saveptr);
        parseLine(token2, rooms[number]->connectedRooms);

        token = strtok_r(NULL, ":", &saveptr);
    }
    free(buff);
    close(fd);

    return rooms;
}

void writeSaveFile(Room **rooms, int size, const char *path)
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
        fprintf(outfile, "%d: ", i);
        Node *p = rooms[i]->connectedRooms->head;
        while (p)
        {
            Node *tmp = p;
            p = p->next;
            fprintf(outfile, "%d ", *(int *)(tmp->data));
        }
        fprintf(outfile, "\n");
    }

    // close file
    fclose(outfile);
}
