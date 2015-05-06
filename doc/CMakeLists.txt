###############################################
# cmake file for building LCIO documentation
# @author Jan Engels, DESY
###############################################

# build documentation out-of-source
SET( DOC_SRC_DIR "${PROJECT_SOURCE_DIR}/doc" )
SET( DOC_BIN_DIR "${PROJECT_BINARY_DIR}/docbuild" )

# tell make clean to erase DOC_BIN_DIR
SET_DIRECTORY_PROPERTIES( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${DOC_BIN_DIR}" )


# add custom target to build all documentation
ADD_CUSTOM_TARGET( doc )


# required packages to build documentation
FIND_PACKAGE( Doxygen REQUIRED )
FIND_PACKAGE( LATEX REQUIRED )


# out-of-source vs in-source installations
IF( "${CMAKE_INSTALL_PREFIX}" STREQUAL "${PROJECT_SOURCE_DIR}" )
    SET( DOC_INSTALL_PREFIX doc )
ELSE()
    SET( DOC_INSTALL_PREFIX doc/lcio )
ENDIF()



# --------- java documentation -----------------------------------------------

IF( INSTALL_JAR )

    ADD_CUSTOM_COMMAND(
        OUTPUT "${DOC_SRC_DIR}/api/index.html"
        COMMAND ${SH} "${LCIO_ENV_INIT}" ant doc
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Building Java API Documentation..."
    )

    ADD_CUSTOM_TARGET( doc_java DEPENDS "${DOC_SRC_DIR}/api/index.html" )
    ADD_DEPENDENCIES( doc_java jar ) # need to generate java headers first 
    ADD_DEPENDENCIES( doc doc_java )

    SET_DIRECTORY_PROPERTIES( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${DOC_SRC_DIR}/api" )

    INSTALL_DIRECTORY( "${DOC_SRC_DIR}/api" DESTINATION ${DOC_INSTALL_PREFIX} )

ENDIF()



# ---------- c++ documentation -----------------------------------------------

IF( DOXYGEN_FOUND )

    ## custom command to generate headers with ant
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT "${LCIO_CXX_HEADERS_DIR}/EVENT/LCIO.h"
    #    COMMAND ${SH} "${LCIO_ENV_INIT}" ant aid.generate
    #    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    #    COMMENT "Generating header files with ant aid.generate..."
    #)

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${DOC_BIN_DIR}/html/index.html"
        COMMAND DOX_PROJECT_NUMBER="${${PROJECT_NAME}_VERSION}"
                DOX_OUTPUT_DIRECTORY="${DOC_BIN_DIR}"
                DOX_INPUT="${LCIO_AID_HEADERS_OUTPUT_DIR}"
                "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY ${DOC_SRC_DIR}/doxygen_api
        COMMENT "Building C++ API Documentation..."
        DEPENDS ./doxygen_api/Doxyfile ./CMakeLists.txt
                ${LCIO_AID_HEADERS_OUTPUT_DIR}/EVENT/*
                ${LCIO_AID_HEADERS_OUTPUT_DIR}/IO/*
    )
    
    ADD_CUSTOM_TARGET( doc_cpp DEPENDS "${DOC_BIN_DIR}/html/index.html" )
    ADD_DEPENDENCIES( doc_cpp lcio ) # need to generate c++ headers first
    ADD_DEPENDENCIES( doc doc_cpp )

    INSTALL_DIRECTORY( "${DOC_BIN_DIR}/html" DESTINATION ${DOC_INSTALL_PREFIX}/doxygen_api )

ELSE()
    MESSAGE( SEND_ERROR "Failed to find doxygen required to build LCIO C++ API documentation" )
    MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
    MESSAGE( "" )
ENDIF()



# ---------- reference manual (generated from doxygen latex) -----------------
IF( DOXYGEN_FOUND AND LATEX_COMPILER AND DVIPS_CONVERTER )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${DOC_BIN_DIR}/latex/lciorefman.ps"
        # FIXME: latex returns error, using | xargs at the end ignores the exit code error
        COMMAND "${LATEX_COMPILER}" -interaction=batchmode refman.tex | xargs
        COMMAND "${LATEX_COMPILER}" -interaction=batchmode refman.tex | xargs
        COMMAND "${DVIPS_CONVERTER}" -q -o refman.ps refman.dvi
        COMMAND psnup -q -2 refman.ps lciorefman.ps # FIXME detect psnup
        WORKING_DIRECTORY "${DOC_BIN_DIR}/latex"
        COMMENT "Building Documentation - Reference Manual..."
    )

    ADD_CUSTOM_TARGET( doc_refman DEPENDS ${DOC_BIN_DIR}/latex/lciorefman.ps )
    ADD_DEPENDENCIES( doc_refman doc_cpp ) # need to generate doxygen documentation first
    ADD_DEPENDENCIES( doc doc_refman )

    INSTALL( FILES "${DOC_BIN_DIR}/latex/lciorefman.ps" DESTINATION ${DOC_INSTALL_PREFIX} )

ELSE()
    MESSAGE( "Failed to find tools required to build lcio reference manual (latex, dvips)" )
    MESSAGE( "Reference manual will not be built")
    MESSAGE( "" )
ENDIF()



# ---------- user manual (latex only) ----------------------------------------
IF( LATEX_COMPILER AND LATEX2HTML_CONVERTER AND PDFLATEX_COMPILER ) # AND DVIPS_CONVERTER )
 
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${DOC_BIN_DIR}/manual"
        # copy directory from source-tree to keep the source-tree clean
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${PROJECT_SOURCE_DIR}/src/latex/manual"
            "${DOC_BIN_DIR}/manual"
    )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${DOC_BIN_DIR}/manual/manual.pdf"
        COMMAND "${LATEX_COMPILER}" -interaction=batchmode manual.tex
        COMMAND "${LATEX_COMPILER}" -interaction=batchmode manual.tex
        #COMMAND "${CMAKE_COMMAND}" -E echo "creating manual.ps"
        #COMMAND "${DVIPS_CONVERTER}" -q -o manual.ps manual.dvi
        COMMAND "${CMAKE_COMMAND}" -E echo "creating manual.pdf"
        COMMAND "${PDFLATEX_COMPILER}" -interaction=batchmode manual.tex
        COMMAND "${CMAKE_COMMAND}" -E echo "creating manual_html"
        COMMAND "${LATEX2HTML_CONVERTER}" -mkdir -dir manual_html -verbosity 0 -info 0 -no_auto_link -split 0 -no_navigation manual.tex
        WORKING_DIRECTORY "${DOC_BIN_DIR}/manual"
        COMMENT "Building Documentation - Users Manual..."
        DEPENDS ${DOC_BIN_DIR}/manual
    )

    ADD_CUSTOM_TARGET( doc_usrman DEPENDS ${DOC_BIN_DIR}/manual/manual.pdf )
    ADD_DEPENDENCIES( doc doc_usrman )


    INSTALL_DIRECTORY( "${DOC_BIN_DIR}/manual/manual_html"
        DESTINATION ${DOC_INSTALL_PREFIX}
    )

    INSTALL( FILES "${DOC_BIN_DIR}/manual/manual.pdf" DESTINATION ${DOC_INSTALL_PREFIX} )

ELSE()
    MESSAGE( "Failed to find tools required to build lcio user manual (latex, latex2html, pdflatex)" )
    MESSAGE( "HTML documentation will not be built")
    MESSAGE( "" )
ENDIF()

# out-of-source vs in-source installations
IF( "${CMAKE_INSTALL_PREFIX}" STREQUAL "${PROJECT_SOURCE_DIR}" )

    # add custom target to keep the source-tree clean
    ADD_CUSTOM_TARGET( uninstall_doc
        COMMAND "${CMAKE_COMMAND}" -E remove_directory "${DOC_SRC_DIR}/api"
        COMMAND "${CMAKE_COMMAND}" -E remove_directory "${DOC_SRC_DIR}/doxygen_api/html"
        COMMAND "${CMAKE_COMMAND}" -E remove_directory "${DOC_SRC_DIR}/manual_html"
    )
ELSE()
    # add custom target to remove whole doc dir
    ADD_CUSTOM_TARGET( uninstall_doc
        COMMAND "${CMAKE_COMMAND}" -E remove_directory
            "${CMAKE_INSTALL_PREFIX}/${DOC_INSTALL_PREFIX}"
    )
ENDIF()

ADD_DEPENDENCIES( uninstall uninstall_doc )

