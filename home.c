#include "save.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#define MAX_NUMBER_OF_ITERATIONS 1000
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
        int random = rand_r(&connection->seed) % count;
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

int main(int argc, char *argv[])
{
    //generateStuff();
    saveStuff();

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

    findRouteFromTo(1, 4, vertices, 10);

    if (!vertices)
    {
        ERR("bad read");
    }

    const char *path2 = "save.save";
    writeSaveFile(vertices, n, path2);

    freeRoomsArray(vertices, n);
}
