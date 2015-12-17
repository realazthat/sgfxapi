cmake_minimum_required(VERSION 2.8)
project(gfxapi)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


set(GLFW3_INCLUDE_DIR ${GLFW3_INCLUDE_DIR} "K:/realz/dump/code/glfw/glfw-3.1.1.bin.WIN32/glfw-3.1.1.bin.WIN32/include")
set(GLFW3_LIB_DIR ${GLFW3_LIB_DIR} "K:/realz/dump/code/glfw/glfw-3.1.1.bin.WIN32/glfw-3.1.1.bin.WIN32/lib-mingw")

set(MGL_INCLUDE_DIR ${MGL_INCLUDE_DIR} "K:/realz/dump/code/mathgeolib/MathGeoLib-master/MathGeoLib-master/src/")
set(MGL_LIB_DIR ${MGL_LIB_DIR} "K:/realz/dump/code/mathgeolib/MathGeoLib-master/MathGeoLib-master/build64")

set(GLU_INCLUDE_DIR ${GLU_INCLUDE_DIR} "")
set(GLU_LIB_DIR ${GLU_LIB_DIR} "")


set(GLUT_INCLUDE_DIR ${GLUT_INCLUDE_DIR} "")
set(GLUT_LIB_DIR ${GLUT_LIB_DIR} "")

set(CUBELIB_INCLUDE_DIR ${CUBELIB_INCLUDE_DIR} "")
set(CUBELIB_LIB_DIR ${CUBELIB_LIB_DIR} "")


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

include_directories(include include/glew src

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
${MGL_LIB_DIR})




#-------------------


#-------------------
add_library(gfxapi src/gfxapi)


target_link_libraries(gfxapi MathGeoLib)



add_executable(gfxdemo src/gfxdemo.cpp src/gfxapi.cpp src/drawutils.cpp)


target_link_libraries(gfxdemo gfxapi MathGeoLib glfw3 opengl32 glu32 glew32 glut)



