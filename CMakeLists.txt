cmake_minimum_required(VERSION 2.8)
project(sgfxapi)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


set(GLFW3_INCLUDE_DIR "K:/realz/dump/code/glfw/glfw-3.1.1.bin.WIN64/glfw-3.1.1.bin.WIN64/include"
    CACHE STRING "Paths to glfw3 includes")
set(GLFW3_LIB_DIR "K:/realz/dump/code/glfw/glfw-3.1.1.bin.WIN64/glfw-3.1.1.bin.WIN64/lib-mingw"
    CACHE STRING "Paths to glfw3 libs")
set(GLFW3_LIB "glfw3"
    CACHE STRING "Lib for glfw3")

set(MGL_INCLUDE_DIR "K:/realz/dump/code/mathgeolib/MathGeoLib-master/MathGeoLib-master/src"
    CACHE STRING "Paths to MathGeoLib includes")
set(MGL_LIB_DIR "K:/realz/dump/code/mathgeolib/MathGeoLib-master/MathGeoLib-master/build64"
    CACHE STRING "Paths to MathGeoLib libs")
set(MGL_LIB "MathGeoLib"
    CACHE STRING "Lib for MathGeoLib")

set(GLU_INCLUDE_DIR "" CACHE STRING "Paths to glut includes")
set(GLU_LIB_DIR "" CACHE STRING "Paths to glut libs")
set(GLU_LIB "glu32" CACHE STRING "Paths to glut libs")
set(GLUT_INCLUDE_DIR "" CACHE STRING "Paths to glut includes")
set(GLUT_LIB_DIR "" CACHE STRING "Paths to glut libs")
set(GLUT_LIB "glut" CACHE STRING "Lib for glut")
set(GLEW_INCLUDE_DIR "" CACHE STRING "Paths to glew includes")
set(GLEW_LIB_DIR "" CACHE STRING "Paths to glew libs")
set(GLEW_LIB "glew32" CACHE STRING "Lib for glew")
set(CUBELIB_INCLUDE_DIR "K:/realz/dump/code/cubelib/repo/include" CACHE STRING "Paths to cubelib includes")
set(CUBELIB_LIB_DIR "" CACHE STRING "Paths to cubelib libs")
set(CUBELIB_LIB "" CACHE STRING "Lib for cubelib")



include(CXX11)
check_for_cxx11_compiler(CXX11_COMPILER)

if(CXX11_COMPILER)
    enable_cxx11()
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} seems to have no C++11 support. Please try again with a more recent compiler version.")
endif()

if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Vc_ARCHITECTURE_FLAGS}")

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CXX_WARNINGS "-Wall -Wextra -Wpointer-arith -Wcast-align -fstrict-aliasing -Wno-unused-local-typedefs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -fvisibility-inlines-hidden")
endif()

#find_package(SDL REQUIRED)

include_directories(include src

${GLFW3_INCLUDE_DIR}
${GLEW_INCLUDE_DIR}
${GLU_INCLUDE_DIR}
${GLUT_INCLUDE_DIR}
${MGL_INCLUDE_DIR}
${CUBELIB_INCLUDE_DIR})
    
link_directories(
${GLFW3_LIB_DIR}
${GLEW_INCLUDE_DIR}
${GLU_INCLUDE_DIR}
${GLUT_LIB_DIR}
${MGL_LIB_DIR}
${CUBELIB_LIB_DIR})




#-------------------


#-------------------
add_library(sgfxapi src/gfxapi)


target_link_libraries(sgfxapi ${MGL_LIB} ${GLFW3_LIB} ${GLU_LIB} ${GLEW_LIB} ${GLUT_LIB} gdi32 opengl32 )


add_library(sgfxapi-drawutils src/drawutils.cpp)


target_link_libraries(sgfxapi-drawutils ${MGL_LIB} ${GLFW3_LIB} ${GLU_LIB} ${GLEW_LIB} ${GLUT_LIB} gdi32 opengl32 )



add_executable(sgfxdemo src/gfxdemo.cpp src/gfxapi.cpp src/drawutils.cpp)


target_link_libraries(sgfxdemo sgfxapi sgfxapi-drawutils ${MGL_LIB}  ${GLFW3_LIB} ${GLU_LIB} ${GLEW_LIB} ${GLUT_LIB}
                        gdi32 opengl32)



