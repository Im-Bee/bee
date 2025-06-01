#!/bin/bash

cmake --log-level=debug .. -DEEPY_PROJECT_NAME="$1"

mv compile_commands.json ..

