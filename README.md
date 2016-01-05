
sgfxapi
---

Simple wrapper around opengl 4, based off of [clb](http://clb.demon.fi)'s [gfxapi docs](http://clb.demon.fi/gfxapi/).

{Long Description}




Dependencies
----

Build system:

* CMake >= 3.1 (if using CMake)

Libraries:

* [MathGeoLib](http://clb.demon.fi/MathGeoLib/nightly/), tested with master from 2015/08/26.
* [glfw3](http://www.glfw.org/docs/latest/), tested with glfw-3.1.1.bin.WIN64 binary distribution.
* [glew](http://glew.sourceforge.net/),
    tested with 1.12.0-3, from the `msys2/mingw-w64 64-bit` environment installed via pacman.
* `glut`, or `freeglut`, tested with `freeglut`, from the `msys2/mingw-w64 64-bit` environment, installed via pacman.
* `glu`
* `corner-cases/cubelib`



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
    
    mkdir build
    
    cd build
    
    cmake -G"MSYS Makefiles" ..
    
    #optionally set include dirs and lib dirs, if they are not installed in the system
    cmake . -DGLFW3_INCLUDE_DIR=/path/to/glfw3/include -DGLFW3_LIB_DIR=/path/to/glfw3/lib
    
    #other variables cmake will understand:
    #MGL_INCLUDE_DIR, MGL_LIB_DIR, MGL_LIB. these are the MathGeoLib paths.
    #GLFW3_INCLUDE_DIR, GLFW3_LIB_DIR, GLFW3_LIB. these are the glfw3 paths.
    #GLU_INCLUDE_DIR, GLU_LIB_DIR, GLU_LIB.
    #GLUT_INCLUDE_DIR, GLUT_LIB_DIR, GLUT_LIB.
    #GLEW_INCLUDE_DIR, GLEW_LIB_DIR, GLEW_LIB.
    #CUBELIB_INCLUDE_DIR.
    #The rest of the required libraries, if any, are expected to be installed in the global
    # include and lib dirs for this compiler. You should also be able to see this list, and
    # any other variables by running `cmake-gui .`
    
    #so for example, if we are using freeglut via msys2, we need to specify the name of the
    # glut library as `freeglut`, but since it is installed in the system, the paths
    # to freeglut can be left out.
    cmake . -DGLUT_LIB=freeglut
    
    #alternatively, you can set these via the GUI
    cmake-gui .
    
    #set the various include and lib directories that are not installed in the system
    #...
    #press configure/generate in the cmake gui, and close the gui.
    
    
    #to compile each target separately:
    make sgfxapi
    make sgfxapi-drawutils
    make sgfxdemo
    
    #to compile them all at once:
    make
    
    #copy over all the lib dlls that are not installed in the system
    cp /path/to/glfw3-libs/glfw3.dll .

    #execute
    ./sgfxdemo

    #if you want this to run outside of the msys2/mingw-w64 environment, then you must additionally copy
    # over any dependant dlls that are part of the environment.
    
    #this displays all the dependancies.
    ldd sgfxdemo

    #copy over any of the results that are part of the environment
    cp /mingw64/bin/libstdc++-6.dll .
    cp /mingw64/bin/libgcc_s_seh-1.dll .
    cp /mingw64/bin/libwinpthread-1.dll .
    cp /mingw64/bin/glew32.dll .
    #and so on

