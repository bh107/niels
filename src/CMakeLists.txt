cmake_minimum_required(VERSION 3.0)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(NielsParser niels.ypp ${CMAKE_CURRENT_BINARY_DIR}/niels.tab.cpp)
FLEX_TARGET(NielsScanner niels.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lfl")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lfl")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(Niels
    driver.cpp
    utils.cpp
    ast.cpp
    symboltable.cpp
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
