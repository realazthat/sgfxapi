
gfxapi
---

{one liner}

{Long Description}




Library Requirements
----



* [MathGeoLib](http://clb.demon.fi/MathGeoLib/nightly/) tested with master from 2015/08/26.
* [glfw3](http://www.glfw.org/docs/latest/)
* [glew32](http://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0-win32.zip/download)
* glut
* glu
* cubelib



How to install some of these via msys2/mingw-w64:

    pacman -S mingw64/mingw-w64-x86_64-freeglut
    pacman -S mingw64/mingw-w64-x86_64-freeglut


Project Parts/Build Targets
----

* gfxapi
* gfxapi-drawutils
* gfxdemo

gfxapi
------

The actual library.


gfxapi-drawutils
------

A utility library for drawing some simple things, and some useful default shaders.


gfxdemo
------

A demo application that renders stuff.



Building
----

Requires C++11.

Tested with `msys2/mingw-w64 64-bit shell` on Windows 7.



#####With codelite:

You can open the 3 projects, `gfxapi.project`, `gfxapi-drawutils.project`, and `gfxdemo`.

You will have to go to each project's settings and set the correct include directories, and the correct linker directories.

Build them in that order.


#####With cmake under msys2/mingw-w64:

Assuming you have a msys2 or linux-like environment, with a gcc-style compiler (clang would
likely also work this way):

    cd gfxapi
    
    mkdir build
    
    cd build
    
    cmake -G"MSYS Makefiles" ..
    
    #optionally set include dirs and lib dirs, if they are not installed in the system
    cmake -D GLFW3_INCLUDE_DIR=/path/to/glfw3/include -DGLFW3_LIB_DIR=/path/to/glfw3/lib .
    
    #other variables cmake will understand:
    #MGL_INCLUDE_DIR, MGL_LIB_DIR. these are the MathGeoLib paths
    #GLFW3_INCLUDE_DIR, GLFW3_LIB_DIR. these are the glfw3 paths.
    #CUBELIB_INCLUDE_DIR.
    #The rest of the required libraries are expected to be installed in the global include and lib dirs
    # for this compiler.
    
    #alternatively, you can set these via the GUI
    cmake-gui .
    
    #set the various include and lib directories that are not installed in the system
    #...
    #press configure/generate in the cmake gui, and close the gui.
    
    
    #to compile each target separately:
    make gfxapi
    make gfxapi-drawutils
    make gfxdemo
    
    #to compile them all at once:
    make










