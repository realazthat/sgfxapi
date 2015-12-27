#!/bin/bash

#####################################################################
####Run this from the project root directory
####This script will delete the library if it is already in the libs directory.
#####################################################################


#exit on failed line
set -exv

PROJECT_PATH=$PWD






#############################################################################
## get/build freeglut
#############################################################################


cd "$PROJECT_PATH"
cd libs

mkdir -p freeglut && cd freeglut

rm -rf ./freeglut/
git clone https://github.com/dcnieho/FreeGLUT.git
cd FreeGLUT
git checkout "575feb5"

mkdir -p build
cd build

cmake -G"$CMAKE_GENERATOR" ..
cmake . -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE"
cmake --build .

