# sop-project

# installation

to start playing run these commands
chmod u+x build.sh;
./build.sh
./home

# commands

list-me
prints where you are and what are your items

list-room
prints what items are in the room and where you can go directly

move-to [roomId]
moves you to the specified room

pick-up [itemId]
if possible picks up the specified otherwise prints an error

drop [itemId]
if possible drops the specified otherwise prints an error

find-path [threadCount] [roomId]
tries to find the path to [roomId] using [threadCount] threads
if succeeded prints the path
otherwise prints an error

quit
quits the game

save
does not work yet
