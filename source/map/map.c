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
int parentId = 0;
int levelOfParent = 0;
int previousLevel = -1;
int previousId = -1;
List *idStack = NULL;
List *lvlStack = NULL;

int countFolders(const char *name, const struct stat *s, int type, struct FTW *f)
{
    if (type == FTW_D)
    {
        printf("[id:%d, lvl:%d] %s", dirs, f->level, name);
        if (f->level > previousLevel)
        {
            addIntItemToList(idStack, previousId);
            addIntItemToList(lvlStack, previousLevel);
        }
        else if (f->level < previousLevel)
        {
            removeFirstItemFromList(idStack);
            removeFirstItemFromList(lvlStack);
        }
        
        printf(" - łączę %d i %d", dirs, firstItemFromList(idStack));
        printf("\n\n");
        previousLevel = f->level;
        previousId = dirs;
        dirs++;
        // previousId = dirs;
        // addIntItemToList(stack, dirs);
        // printRoute(stack);
        // if (f->level != levelOfParent)
        // {
        //     levelOfParent = f->level;
        //     parentId = dirs;
        // }
        // if (f->level > levelOfParent)
        // {
        //     removeFirstItemFromList(stack);
        //     printRoute(stack);
        // }
        // printf("my name is %s level is %d\n", name, f->level);
        // printf("my id is %d, my parent is %d\n\n", dirs, parentId);
        // printf("base is %s\n", name + f->base);
    }
    return 0;
}

int addRoomsFromFolders(const char *name, const struct stat *s, int type, struct FTW *f)
{
    if (type == FTW_D)
    {
    }
    return 0;
}

void scan_dir(const char *path)
{
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
    if (NULL == (dirp = opendir(path)))
        ERR("opendir");
    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            if (lstat(dp->d_name, &filestat))
                ERR("lstat");
            if (S_ISDIR(filestat.st_mode))
            {
                if (strcmp(".", dp->d_name) != 0 && strcmp("..", dp->d_name) != 0)
                {
                    printf("%s\n", dp->d_name);
                }
            }
        }
    } while (dp != NULL);
    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
}

void mapFromDirTree(Room ***roomsPtr, const char *path)
{
    idStack = newList();
    lvlStack = newList();

    nftw(path, countFolders, MAXFD, FTW_PHYS);

    // Room **rooms = createRooms(dirs);

    // nftw(path, countFolders, MAXFD, FTW_PHYS);
    // printRooms(rooms, dirs);
    // freeRoomsArray(rooms, dirs);
}