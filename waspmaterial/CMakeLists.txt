
INCLUDE(WaspAddCompilerFlags)

TRIBITS_SUBPACKAGE(material)
SET_DEFAULT(DBC 0)
SET_DEFAULT(TIMING 0)
SET_DEFAULT(DEBUG_OUTPUT 0)
##---------------------------------------------------------------------------##
## INCLUDES

##---------------------------------------------------------------------------##
## HEADER AND SOURCE FILES
##---------------------------------------------------------------------------##

WASP_ADD_CXX_FLAGS(
        "-Wno-deprecated-register"
)

# C/C++ headers
FILE(GLOB HEADERS *.h *.hh
        "${CMAKE_CURRENT_BINARY_DIR}/*.h"
        )

# c++ files
SET(SOURCES
        MaterialDatabase.cpp
        )

##---------------------------------------------------------------------------##
## PACKAGE TARGETS AND INSTALL
##---------------------------------------------------------------------------##

TRIBITS_ADD_LIBRARY(
        waspmaterial
        SOURCES ${SOURCES}
        NOINSTALLHEADERS ${HEADERS})

INSTALL(FILES
        ${HEADERS}
        DESTINATION
        include/waspmaterial)

#
# Add test directory for this package
#
TRIBITS_ADD_TEST_DIRECTORIES(test)

##---------------------------------------------------------------------------##
##                   end of JSON/CMakeLists.txt
##---------------------------------------------------------------------------##
TRIBITS_SUBPACKAGE_POSTPROCESS()
