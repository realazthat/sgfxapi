

set(DOXYGEN_EXECUTABLE "doxygen" CACHE STRING "Path to doxygen executable")
set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

set(sphinx_conf_in ${CMAKE_CURRENT_SOURCE_DIR}/source/conf.py)
set(sphinx_conf ${CMAKE_CURRENT_BINARY_DIR}/source/conf.py)

#configure_file(${sphinx_conf_in} ${sphinx_conf} @ONLY)
configure_file(${sphinx_conf_in} ${sphinx_conf} @ONLY)



add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

#see http://stackoverflow.com/a/11249722/586784

macro(add_sphinx_target TARGET_NAME BUILDER COMMENT_STR)
    add_custom_target(${TARGET_NAME}
    COMMAND sphinx-build
                    -E
                    -D"breathe_projects.sgfxapi=../doxyxml"
                    -c ${CMAKE_CURRENT_BINARY_DIR}/source/
                    -b ${BUILDER}
                    ${CMAKE_CURRENT_SOURCE_DIR}/source  ${CMAKE_CURRENT_BINARY_DIR}/sphinx/${BUILDER}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS doxygen
        COMMENT ${COMMENT_STR}
    )

endmacro(add_sphinx_target)


add_sphinx_target(docs-html
    html
    "Build html documentation"
)

