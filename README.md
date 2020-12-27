# sop-project

## installation

to start playing run these commands
<<<<<<< HEAD
=======

# prerequisites

`chmod u+x build.sh`

# compilation

`./build.sh`

# play

`build/sop_game`
>>>>>>> 4aa054243f62f4265d041d44639e627a9ecc7e59

### prerequisites

`chmod u+x build.sh`

### compilation

`./build.sh`

### play

`build/sop_game`

### commands

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
