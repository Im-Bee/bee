#!/bin/bash

if [ ! -e ./Engine/Build/CMakeLists.txt ]; then
    echo "Please run the scrpit from the root directory of the Duckers."
    exit
fi

if [ -e Bin ]; then
    rm -r Bin
fi

if [ -e Engine/CMake ]; then
    rm -r Engine/CMake
fi

if [ -e Engine/Lib ]; then
    rm -r Engine/Lib
fi

if [ -e compile_commands.json ]; then
    rm compile_commands.json
fi
