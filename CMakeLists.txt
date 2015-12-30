cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(sgfxapi)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


set(GLFW3_INCLUDE_DIR "./libs/glfw/glfw/include"
    CACHE STRING "Paths to glfw3 includes")
set(GLFW3_LIB_DIR "./libs/glfw/glfw/build"
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
set(CUBELIB_INCLUDE_DIR "./libs/cubelib/cubelib/include" CACHE STRING "Paths to cubelib includes")
set(CUBELIB_LIB_DIR "" CACHE STRING "Paths to cubelib libs")
set(CUBELIB_LIB "" CACHE STRING "Lib for cubelib")




if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()


if (CMAKE_COMPILER_IS_GNUCXX)
    set(CXX_WARNINGS "-Wall -Wextra -Wpointer-arith -Wcast-align -fstrict-aliasing -Wno-unused-local-typedefs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -fvisibility-inlines-hidden")
endif()


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












set(DOCS_HEADERS include/sgfxapi/sgfxapi.hpp include/sgfxapi/sgfxapi-drawutils.hpp include/sgfxapi/sgfxdemo.hpp)
set(DOCS_SOURCES src/sgfxapi/sgfxapi.cpp src/sgfxapi/sgfxapi-drawutils.cpp)


set(CLDOC "cldoc" CACHE STRING "User specified '/path/to/cldoc'")

if(NOT DOCS_OUTPUT_DIR)
    set(DOCS_OUTPUT_DIR "gendocs" CACHE STRING "Generated documentation output directory" FORCE)
endif()

if(NOT DOCS_DIR)
    set(DOCS_DIR "" CACHE STRING "Location of external documentation files to be merged in" FORCE)
endif()

if(NOT ${CLDOC} STREQUAL "CLDOC-NOTFOUND")
    if(NOT ${DOCS_DIR} STREQUAL "")
        set(CLDOC_MERGE "--merge ${DOCS_DIR}")
    else()
        set(CLDOC_MERGE "")
    endif()




    set(CLDOC_CMAKE_CXX_FLAGS "-std=c++11")
    set(CLDOC_GENERATE_SCRIPT_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/build-docs.in")
    set(CLDOC_GENERATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/build-docs.cmake")
     
    #file(WRITE ${CLDOC_GENERATE_SCRIPT_TEMPLATE} [[
    #    set(MY_INCLUDES "$<TARGET_PROPERTY:sgfxdemo,INCLUDE_DIRECTORIES>")
    #    message("My include directories are: ${MY_INCLUDES}")
    #]])
    
    
    configure_file("${CLDOC_GENERATE_SCRIPT_TEMPLATE}" "${CLDOC_GENERATE_SCRIPT}" @ONLY)
    
    #set(CLDOC_INCLUDE_FLAGS "-I$<JOIN:$<TARGET_PROPERTY:sgfxdemo,INCLUDE_DIRECTORIES>, -I>")
    file(GENERATE OUTPUT ${CLDOC_GENERATE_SCRIPT}
        INPUT ${CLDOC_GENERATE_SCRIPT}
    )
     
    add_custom_target(docs ALL
        COMMAND ${CMAKE_COMMAND} -P ${CLDOC_GENERATE_SCRIPT}
    )

    

    set_target_properties(docs PROPERTIES
        EXCLUDE_FROM_ALL 1
        EXCLUDE_FROM_DEFAULT_BUILD 1
    )

    add_custom_target(serve_docs COMMAND
        ${CLDOC} serve -- ${DOCS_OUTPUT_DIR}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    set_target_properties(docs PROPERTIES
        EXCLUDE_FROM_ALL 1
        EXCLUDE_FROM_DEFAULT_BUILD 1
    )

    add_dependencies(serve_docs docs)
else()
    message(STATUS
    "Documentation will not be generated"
    )
endif()








