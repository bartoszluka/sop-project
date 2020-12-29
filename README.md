# sop-project

## installation

### prerequisites

`chmod u+x build.sh`

### compilation

`./build.sh`

### play

`build/sop_game`

## main menu commands

`map-from-dir-tree śckieżka-d ścieżka-f`

generuje mapę odwzorowującą drzewo katalogów od ścieżka-d w dół, gdzie każdy katalog to pomieszczenie, a przejścia pomiędzy pomieszczeniami są pomiędzy katalogiem nadrzędnym, a katalogami w środku. Wynikowa mapa zapisywana jest do pliku ścieżka-f.

`generate-random-map n ścieżka`

generuje losową mapę złożoną z n pomieszczeń, pomieszczenia są połączone w sposób losowy. Sposób losowania połączeń należy zaproponować samodzielnie. Wynikowa mapa jest zapisywana do pliku ścieżka. Mapa powinna być grafem spójnym.

`start-game ścieżka`

uruchamia nową grę na mapie z pliku sciezka

`load-game ścieżka`

ładuje stan gry z pliku ścieżka (zarówno mapa, jak i stan przedmiotów i gracza).

`exit`
wychodzi z programu

## in-game commands

`list-me`

prints where you are and what are your items

`list-room`

prints what items are in the room and where you can go directly

`move-to [roomId]`

moves you to the specified room

`pick-up [itemId]`

if possible picks up the specified otherwise prints an error

`drop [itemId]`

if possible drops the specified otherwise prints an error

`find-path [threadCount] [roomId]`

tries to find the path to [roomId] using [threadCount] threads
if succeeded prints the path
otherwise prints an error

`quit`

quits the game

`save`

saves current game state
