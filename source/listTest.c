#include "game/game.h"



void itemsTest()
{
    int arraySize = 10;
    Room **rooms = createRooms(arraySize);

    printRooms(rooms, arraySize);
    addItemsToRooms(rooms, arraySize);
    puts("=====after=====");
    printRooms(rooms, arraySize);

    freeRoomsArray(rooms, arraySize);
}

int main(int argc, char *argv[])
{

    return 0;
}