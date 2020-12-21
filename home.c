#include "save.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#define MAX_NUMBER_OF_ITERATIONS 1000
void saveStuff();

void printRoom(Room *room)
{
    printf("room number %d:\n", room->roomId);
    printf("list: \n");
    printIntList(room->connectedRooms);
    printf("items: \n");
    for (int j = 0; j < ITEMS_IN_ROOM; j++)
    {
        printItem(room->items[j]);
    }
}

void printRooms(Room *rooms[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        printRoom(rooms[i]);
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

typedef struct routeArguments
{
    Room **rooms;
    int u;
    int v;
    unsigned int seed;
} routeArguments;

typedef struct routeResult
{
    List *route;
    int exists;
} routeResult;

void *findRoute(void *args)
{
    routeArguments *connection = (routeArguments *)args;
    int iterations = 0;
    int currentRoom = connection->u;
    routeResult *result = (routeResult *)malloc(sizeof(routeResult));
    if (!result)
    {
        ERR("malloc");
    }
    result->exists = 0;
    result->route = newList();
    addIntsToList2(result->route, currentRoom);

    while (iterations < MAX_NUMBER_OF_ITERATIONS &&
           currentRoom != connection->v)
    {

        Node *p = connection->rooms[currentRoom]->connectedRooms->head;
        int count = connection->rooms[currentRoom]->connectedRooms->Count;
        int r = rand_r(&connection->seed);
        int random = r % count;
        for (int i = 0; i < random; i++)
        {
            p = p->next;
        }
        currentRoom = *(int *)(p->data);

        addIntsToList2(result->route, currentRoom);
        iterations++;
    }
    if (iterations < MAX_NUMBER_OF_ITERATIONS)
    {
        result->exists = 1;
    }
    return result;
}

void generateConnections(Room **rooms, int arraySize)
{
    srand(time(NULL));
    unsigned int pid = getpid();
    int rand = rand_r(&pid) % (arraySize);

    int *permutation = (int *)calloc(sizeof(int), arraySize);
    for (int i = 0; i < arraySize; i++)
    {
        permutation[rand] = i;
        rand = rand_r(&pid) % (arraySize);
    }
    free(permutation);
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

void gamerStuff()
{

    Gamer *gamer = newGamer(0);
    gamer->items[0] = newItem(1, 2);
    gamer->items[1] = newItem(3, 4);
    printGamer(gamer);
    freeGamer(gamer);
}
int hasSlot(Item **items)
{
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (!items[i])
        {
            return i;
        }
    }
    return -1;
}

int gamerHasSlot(Gamer *gamer)
{
    return hasSlot(gamer->items);
}

int roomHasSlot(Room *room)
{
    return hasSlot(room->items);
}

int findItemInItems(Item **items, int itemId)
{
    for (int i = 0; i < ITEMS_IN_ROOM; i++)
    {
        if (items[i] && items[i]->itemId == itemId)
        {
            return i;
        }
    }
    printf("there is no such item in this room\n");
    return -1;
}
void pickItem(Gamer *gamer, Room *room, int itemId)
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

void dropItem(Gamer *gamer, Room *room, int eqItemId)
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
}

void game()
{
    int n;
    const char *path = "save2.save";
    // scanf("%d", &n);
    Room **rooms;
    rooms = readSaveFile(path, &n);

    if (!rooms)
    {
        ERR("bad read");
    }
    // findRouteFromTo(1, 4, vertices, 10);

    generateRandomMap(rooms, n);
    srand(time(NULL));
    int startPosition = rand() % n;
    Gamer *gamer = newGamer(startPosition);
    printGamer(gamer);
    printRoom(rooms[gamer->position]);

    while (1)
    {

        int optionInt;
        char *option;
        scanf("%s", option);
        printf("%s\n", option);
        scanf("%d", &optionInt);

        if (strcmp("move-to", option) == 0)
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
        else if (strcmp("pick-up", option) == 0)
        {
            int slot = findItemInItems(rooms[gamer->position]->items, optionInt);
            if (slot != -1)
            {
                printf("you picked up the item\n");
                printItem(rooms[gamer->position]->items[slot]);
                pickItem(gamer, rooms[gamer->position], slot);
            }
            else
            {
                printf("you can't pick up this item\n");
                printf("items you can pick up are:\n");
                printAllItems(rooms[gamer->position]->items);
            }
        }

        else if (strcmp("drop", option) == 0)
        {
            int slot = findItemInItems(gamer->items, optionInt);
            if (slot != -1)
            {
                printf("you successfully dropped the item\n");
                printItem(gamer->items[slot]);
                dropItem(gamer, rooms[gamer->position], slot);
            }
            else
            {
                printf("you can't drop this item here\n");
                printf("this room is full\n");
            }
        }
        else if (strcmp("quit", option) == 0)
        {
            break;
        }
    }
}
int main(int argc, char *argv[])
{
    //generateStuff();
    // saveStuff();
    game();

    // gamerStuff();
    // free(rooms);
    return 0;
}

void findRouteFromTo(int from, int to, Room **rooms, int threadCount)
{
    srand(time(NULL));
    routeArguments args;
    args.rooms = rooms;
    args.u = to;
    args.v = from;
    args.seed = rand();
    int numberOfWorkers = threadCount;
    pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * numberOfWorkers);

    for (int i = 0; i < numberOfWorkers; i++)
    {
        pthread_create(&tids[i], NULL, findRoute, &args);
    }
    int found = 0;
    List *shortestRoute = newList();
    int minDist = 1000;
    for (int i = 0; i < numberOfWorkers; i++)
    {
        void *resultPtr;
        pthread_join(tids[i], &resultPtr);
        routeResult *result = (routeResult *)resultPtr;
        found += result->exists;
        if (result->exists && result->route->Count < minDist)
        {
            minDist = result->route->Count;
            shortestRoute->Count = result->route->Count;
            shortestRoute->head = result->route->head;
        }
        free(result);
    }
    if (!found)
    {
        printf("route from %d to %d does not exist\n", from, to);
    }
    else
    {
        printf("route from %d to %d exist\n", from, to);
        printf("its length is %d\n", shortestRoute->Count);
        printRoute(shortestRoute);
    }
    free(tids);
    free(shortestRoute);
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
    // findRouteFromTo(1, 4, vertices, 10);

    generateRandomMap(vertices, n);
    srand(time(NULL));
    int startPosition = rand() % n;
    Gamer *gamer = newGamer(startPosition);
    printGamer(gamer);
    printRoom(vertices[gamer->position]);

    // int itemIndex = findItemInItems(vertices[gamer->position], 3);
    int itemIndex = 0;
    pickItem(gamer, vertices[gamer->position], itemIndex);

    printf("-------\n");
    printGamer(gamer);
    printRoom(vertices[gamer->position]);

    int gamerItemIndex = 0;
    dropItem(gamer, vertices[gamer->position], gamerItemIndex);

    printf("-------\n");
    printGamer(gamer);
    printRoom(vertices[gamer->position]);

    const char *path2 = "save.save";
    writeSaveFile(vertices, n, path2);

    freeRoomsArray(vertices, n);
}
