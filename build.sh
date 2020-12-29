#!/bin/bash
# 
# Execute this line to make it executable:
# chmod u+x build.sh

# Create some space between compiler warnings and program output
echo

# Compile everything that is needed
mkdir -p build
make -C build -f ../Makefile


# Run build
# build/sop_game

# Create some space again
echo