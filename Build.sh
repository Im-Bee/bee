#!/bin/bash

if [ ! -e ./Engine/Build/CMakeLists.txt ]; then
    echo "Please run the scrpit from the root directory of the Duckers."
    exit
fi

if [ "$1" = "" ]; then
    echo "To use the scrpit provide either project name or sample name:"
    echo "./Build.sh [--Sample] <Project/sample name>"
    exit
fi

if [ "$1" = "--Sample" ]; then
    if [ "$2" = "" ]; then
        echo "Please provide sample name after '--Sample'."
        exit
    fi

    if [ ! -e ./Engine/CMake/"$2" ]; then
        mkdir -p ./Engine/CMake/"$2"
    fi

    cd Engine/CMake/"$2"
    
    if  find ./ -maxdepth 0 -empty; then
        cmake --log-level=debug ../../Build/CMakeLists.txt -DDUCKERS_LOG_DATA=TRUE -DDUCKERS_TARGET_SAMPLE_NAME="$2"
    fi
else

    if [ ! -e Engine/CMake/"$1" ]; then
        mkdir -p Engine/CMake/"$1"
    fi

    cd Engine/CMake/"$1"

    if  find ./ -maxdepth 0 -empty; then
        cmake --log-level=verbose ../../Build/CMakeLists.txt -DDUCKERS_LOG_DATA=TRUE -DDUCKERS_TARGET_PROJECT_NAME="$1"
    fi
fi

make
mv compile_commands.json ./../../..
cd ../..

