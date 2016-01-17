
sgfxapi
---

Simple wrapper around opengl 4, based off of [clb](http://clb.demon.fi)'s [gfxapi docs](http://clb.demon.fi/gfxapi/).

The wrapper can help users understand the requirements of OpenGL4 API and to learn the OpenGL4 API.


License
----
Libraries have their own license.

Some specific files might have their own license included in them; in this case those files fall under
that license. Some directories might have their own license; in that case those directories will have
a README, LICENSE, COPYING or similar file related to licensing.

As for the rest of the project, code written by me in this project is released under the
[MIT License](https://opensource.org/licenses/MIT).


Dependencies
----

Build system:

* CMake >= 3.1 (if using CMake)
* doxygen for documentation generation
* sphinx (python) for documentation generation
* breathe (python) for documentation generation

Libraries:

* [MathGeoLib](http://clb.demon.fi/MathGeoLib/nightly/), tested with master from 2015/08/26.
* [glfw3](http://www.glfw.org/docs/latest/), tested with glfw-3.1.1.bin.WIN64 binary distribution.
* [glew](http://glew.sourceforge.net/),
    tested with 1.12.0-3, from the `msys2/mingw-w64 64-bit` environment installed via pacman.
* `glut`, or `freeglut`, tested with `freeglut`, from the `msys2/mingw-w64 64-bit` environment, installed via pacman.
* `glu`
* [corner-cases/cubelib](https://github.com/realazthat/corner-cases)
    * Useful library to reason about corners, edges, sides, of an imaginary cube



How to install some of these via msys2/mingw-w64:

    #search
    pacman -Ss glut
    
    #install
    pacman -S mingw64/mingw-w64-x86_64-freeglut


Project Parts/Build Targets
----

* sgfxapi
* sgfxapi-drawutils
* sgfxdemo

sgfxapi
------

The actual library.


sgfxapi-drawutils
------

A utility library for drawing some simple things, and some useful default shaders.


sgfxdemo
------

A demo application that renders stuff.



Building
----

Requires C++11.

Tested with `msys2/mingw-w64 64-bit shell` on Windows 7.



#####With codelite:

You can open the 3 projects, `sgfxapi.project`, `sgfxapi-drawutils.project`, and `sgfxdemo.project`.

You will have to go to each project's settings and set the correct include directories, and the correct linker directories.

Build them in that order.

Copy any dlls dependencies to the build directories to run. You will need to copy the ones for the lib dependencies
and also the ones that are part of the compiler runtime, if any.

    #these are for gcc, it would depend on the compiler and environment if you need to copy these over.
    cp /path/to/compiler/libstdc++-6.dll .
    cp /path/to/compiler/libgcc_s_seh-1.dll .
    cp /path/to/compiler/libwinpthread-1.dll .
    #and these are the library depndencies
    cp /path/to/glew/glew32.dll .
    cp /path/to/glfw/gflw3.dll .
    #and so on

If you still get errors about missing dlls, or see a windows error of `0xc000007b` (whichmeans that one of the
dlls are not in the build directory, and instead it found installed on the system; but the one found is an
architecture mismatch (32-bit/64-bit) ) then you have to find what other runtime dlls your binary requires.
If using an msys-based compiler, you can use msys2's `ldd` (find via `pacman -Ss ldd` and install via something
like `pacman -S mingw64/mingw-w64-x86_64-ntldd-git`) from within msys2 to locate these dlls, or use
[Dependency Walker](http://www.dependencywalker.com/) to locate them within the windows environment.




#####With cmake under msys2/mingw-w64:

Assuming you have a msys2 or linux-like environment, with a gcc-style compiler (clang would
likely also work this way):

    cd sgfxapi
    
    
    
    #install dependencies, there are some bash scripts provided in the ./scripts/ directory
    # that will download and build many of the dependencies and put them in the ./libs directory
    # the scripts are meant for the continuous integration system, but you can run them yourself
    # or read them for assistance.
    
    #optionally install or download and build googletest (only matters if you are going to run the unittests)
    #note, choose the appropriate generator
    #see the bash scripts for for more details
    CMAKE_GENERATOR="MSYS Makefiles" CMAKE_BUILD_TYPE="Debug" bash ./scripts/download-and-build-googletest.sh
    #... etc. install dependencies
    
    
    #make a build directory
    mkdir -p build && cd build
    
    
    #note, choose the appropriate generator
    cmake -G"MSYS Makefiles" .. -DCMAKE_BUILD_TYPE="Debug"
    
    #if you built the dependencies and put them in the libs directory, then you are good to build
    
    #if you installed the dependencies to the system, you are prolly good to build

    #if you built the dependencies yourself outside the expected ./libs subdirectories, then you will need to
    # define/override the paths to the projects (which by default point to the ./libs directory)
    # for example, like so:
    cmake -L # list all the user-definable variables
    cmake . -DGLFW3_INCLUDE_DIR=/path/to/cppformat/include -DGLFW3_LIB=glfw3 -DGLFW3_LIB_DIR=/path/to/glfw/build/src
    # .. and so on for each dependency that is not in the ./libs directory and not installed in the system

    
    #furthermore, if we are using freeglut via msys2, we need to specify the name of the
    # glut library as `freeglut`
    cmake . -DGLUT_LIB=freeglut
    
    #alternatively, we can set these via the GUI
    cmake-gui .
    
    #set the various include and lib directories that are not installed in the system
    #...
    #press configure/generate in the cmake gui, and close the gui.
    
    
    #to compile each target separately:
    cmake --build . --target sgfxapi
    cmake --build . --target sgfxapi-drawutils
    cmake --build . --target sgfxdemo
    
    #to compile them all at once:
    cmake --build .
    
    #copy over all the lib dlls that are not installed in the system
    cp /path/to/glfw3-libs/glfw3.dll .
    # ...etc.

    #execute
    ./sgfxdemo

    #if you want this to run outside of the msys2/mingw-w64 environment, then you must additionally copy
    # over any dependant dlls that are part of the mingw environment.
    
    #this displays all the dependencies.
    ldd sgfxdemo

    #copy over any of the results that are part of the environment
    cp /mingw64/bin/libstdc++-6.dll .
    cp /mingw64/bin/libgcc_s_seh-1.dll .
    cp /mingw64/bin/libwinpthread-1.dll .
    cp /mingw64/bin/glew32.dll .
    #and so on

