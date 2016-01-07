#!/bin/bash

#####################################################################
####Run this from the project root directory
####This script will delete the library if it is already in the libs directory.
#####################################################################


#exit on failed line
set -exv

PROJECT_PATH=$PWD






#############################################################################
## get/build glfw
#############################################################################


cd "$PROJECT_PATH"
cd libs

mkdir -p glfw3 && cd glfw3

rm -rf ./glfw/
git clone https://github.com/glfw/glfw.git
cd glfw
git checkout "cf6c11c"

mkdir -p build
cd build

cmake -G"$CMAKE_GENERATOR" ..
cmake . -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE"
cmake . -DCMAKE_VERBOSE_MAKEFILE=1
cmake --build . --target glfw

