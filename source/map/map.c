#include "map.h"

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
    addIntItemToList(result->route, currentRoom);

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
        currentRoom = (p->data);

        addIntItemToList(result->route, currentRoom);
        iterations++;
    }
    if (iterations < MAX_NUMBER_OF_ITERATIONS)
    {
        result->exists = 1;
    }
    return result;
}

void findRouteFromTo(int from, int to, Room **rooms, int size, int threadCount)
{
    srand(time(NULL));
    routeArguments args;
    args.rooms = rooms;
    if (to > size || to < 0)
    {
        printf("room %d does not exist\n", to);
        return;
    }
    args.u = from;
    args.v = to;
    args.seed = rand();
    int numberOfWorkers = threadCount;
    pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * numberOfWorkers);

    for (int i = 0; i < numberOfWorkers; i++)
    {
        pthread_create(&tids[i], NULL, findRoute, &args);
    }
    int found = 0;
    List *shortestRoute = NULL;
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
            if (shortestRoute)
            {
                freeList(shortestRoute);
            }
            shortestRoute = newList();
            copyList(result->route, shortestRoute);
        }
        freeList(result->route);
        free(result);
    }
    if (!found)
    {
        printf("route from %d to %d does not exist\n", from, to);
    }
    else
    {
        printRoute(shortestRoute);
        freeList(shortestRoute);
    }
    free(tids);
}

int dirs = 0;

int forEachFolder(const char *name, const struct stat *s, int type, struct FTW *f)
{
    if (type == FTW_D)
    {
        dirs++;
        printf("%d ", dirs);
        printf("size of %s is %ld\n", name, s->st_size);
    }
    return 0;
}

void mapFromDirTree(Room ***roomsPtr, const char *path)
{
    nftw(path, forEachFolder, MAXFD, FTW_PHYS);
}