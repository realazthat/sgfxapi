cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(sgfxapi)



set(GLFW3_INCLUDE_DIR "./libs/glfw/glfw/include"
    CACHE STRING "Paths to glfw3 includes")
set(GLFW3_LIB_DIR "./libs/glfw3/glfw/build"
    CACHE STRING "Paths to glfw3 libs")
set(GLFW3_LIB "glfw3"
    CACHE STRING "Lib for glfw3")

set(MGL_INCLUDE_DIR "./libs/mathgeolib/MathGeoLib/src"
    CACHE STRING "Paths to MathGeoLib includes")
set(MGL_LIB_DIR "./libs/mathgeolib/MathGeoLib/build"
    CACHE STRING "Paths to MathGeoLib libs")
set(MGL_LIB "MathGeoLib"
    CACHE STRING "Lib for MathGeoLib")

set(GLU_INCLUDE_DIR "" CACHE STRING "Paths to glut includes")
set(GLU_LIB_DIR "" CACHE STRING "Paths to glut libs")
set(GLU_LIB "glu32" CACHE STRING "Paths to glut libs")
set(GLUT_INCLUDE_DIR "./libs/freeglut/FreeGLUT/build;./libs/freeglut/FreeGLUT/freeglut/freeglut/include" CACHE STRING "Paths to glut includes")
set(GLUT_LIB_DIR "./libs/freeglut/FreeGLUT/build/lib" CACHE STRING "Paths to glut libs")
set(GLUT_LIB "glut" CACHE STRING "Lib for glut")
set(GLEW_INCLUDE_DIR "" CACHE STRING "Paths to glew includes")
set(GLEW_LIB_DIR "" CACHE STRING "Paths to glew libs")
set(GLEW_LIB "glew32" CACHE STRING "Lib for glew")
set(CUBELIB_INCLUDE_DIR "./libs/corner-cases/corner-cases/include" CACHE STRING "Paths to corner-cases/cubelib includes")
set(CUBELIB_LIB_DIR "" CACHE STRING "Paths to corner-cases/cubelib libs")
set(CUBELIB_LIB "" CACHE STRING "Lib for corner-cases/cubelib")



#
if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()


if (CMAKE_COMPILER_IS_GNUCXX)
    set(CXX_WARNINGS "-Wall -Wextra -Wpointer-arith -Wcast-align -fstrict-aliasing -Wno-unused-local-typedefs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -fvisibility-inlines-hidden")
endif()

add_subdirectory(docs)


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





################################################################################
add_library(sgfxapi src/sgfxapi/sgfxapi.cpp)

target_link_libraries(sgfxapi ${MGL_LIB} ${GLFW3_LIB} ${GLU_LIB} ${GLEW_LIB} ${GLUT_LIB} gdi32 opengl32 )

set_property(TARGET sgfxapi PROPERTY CXX_STANDARD 11)
################################################################################
add_library(sgfxapi-drawutils src/sgfxapi/sgfxapi-drawutils.cpp)

target_link_libraries(sgfxapi-drawutils ${MGL_LIB} ${GLFW3_LIB} ${GLU_LIB} ${GLEW_LIB} ${GLUT_LIB} gdi32 opengl32 )

set_property(TARGET sgfxapi-drawutils PROPERTY CXX_STANDARD 11)
################################################################################

add_executable(sgfxdemo src/sgfxapi/sgfxdemo.cpp )


target_link_libraries(sgfxdemo sgfxapi sgfxapi-drawutils ${MGL_LIB}
    ${GLFW3_LIB} ${GLU_LIB} ${GLEW_LIB} ${GLUT_LIB} gdi32 opengl32)

set_property(TARGET sgfxdemo PROPERTY CXX_STANDARD 11)
################################################################################















