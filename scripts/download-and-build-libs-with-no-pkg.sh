#!/bin/bash

#####################################################################
####Run this from the project root directory
####This script will delete/overwrite any libraries in the ./libs directory
#### that it will download and build.
####
####Usage (on linux):
####  CMAKE_GENERATOR="Unix Makefiles" bash ./scripts/download-and-build-libs-with-no-pkg.sh
#####################################################################


#exit on failed line
set -exv

PROJECT_PATH=$PWD



#############################################################################
## get/build cubelib
#############################################################################

cd "$PROJECT_PATH"
bash ./scripts/download-and-build-cubelib.sh


#############################################################################
## get/build mathgeolib
#############################################################################

cd "$PROJECT_PATH"
bash ./scripts/download-and-build-mathgeolib.sh


#############################################################################
## get/build freeglut
#############################################################################

cd "$PROJECT_PATH"
bash ./scripts/download-and-build-freeglut.sh

