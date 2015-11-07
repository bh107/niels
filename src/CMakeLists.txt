cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(NielsParser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(NielsScanner scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

set(BOHRIUM_LIB "$ENV{HOME}/.local/lib" )
set(BOHRIUM_INC "$ENV{HOME}/.local/include" )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${BOHRIUM_INC}/")
include_directories("${BOHRIUM_INC}/bohrium")
link_directories("${BOHRIUM_LIB}")

add_executable(niels
    driver.cc
    utils.cc
    ast.cc
    symboltable.cc
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
target_link_libraries(niels bh bhc)
