cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

SET(BOHRIUM_LIB "$ENV{HOME}/.local/lib" CACHE STRING "Bohrium lib path")
SET(BOHRIUM_INC "$ENV{HOME}/.local/include" CACHE STRING "Bohrium inc path")

SET(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
SET(GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/autogen")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include_directories(${SRC_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("${BOHRIUM_INC}/")
include_directories("${BOHRIUM_INC}/bohrium")
link_directories("${BOHRIUM_LIB}")

BISON_TARGET(NielsParser "${SRC_DIR}/parser.yy" ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(NielsScanner "${SRC_DIR}/scanner.ll" ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

set(HDR "${HDR};${SRC_DIR}/nls/nls.hh")
set(HDR "${HDR};${SRC_DIR}/nls/variant.hh")
set(HDR "${HDR};${SRC_DIR}/nls/utils.hh")
set(HDR "${HDR};${SRC_DIR}/nls/driver.hh")
set(HDR "${HDR};${SRC_DIR}/nls/symbol_table.hh")
set(HDR "${HDR};${SRC_DIR}/nls/ast/node.hh")

set(SRC "${SRC};${SRC_DIR}/niels.cc")

set(SRC "${SRC};${SRC_DIR}/nls/utils.cc")
set(SRC "${SRC};${SRC_DIR}/nls/driver.cc")
set(SRC "${SRC};${SRC_DIR}/nls/symbol_table.cc")
set(SRC "${SRC};${SRC_DIR}/nls/ast/node.cc")

add_custom_command(OUTPUT ${SRC_DIR}/nls/ast/expr_binary_auto.hh ${SRC_DIR}/nls/ast/expr_unary_auto.cc ${SRC_DIR}/nls/ast/expr_binary_auto.cc ${SRC_DIR}/nls/ast/expr_unary_auto.hh

    COMMAND ./niels.py > niels.json
    COMMAND ./gen.py --generator=expr_unary_auto.hh > ${SRC_DIR}/nls/ast/expr_unary_auto.hh
    COMMAND ./gen.py --generator=expr_unary_auto.cc > ${SRC_DIR}/nls/ast/expr_unary_auto.cc
    COMMAND ./gen.py --generator=expr_binary_auto.hh > ${SRC_DIR}/nls/ast/expr_binary_auto.hh
    COMMAND ./gen.py --generator=expr_binary_auto.cc > ${SRC_DIR}/nls/ast/expr_binary_auto.cc

    WORKING_DIRECTORY ${GEN_DIR}
    DEPENDS ${GEN_DIR}/templates/expr_binary_auto_hh.tpl ${GEN_DIR}/templates/expr_binary_auto_cc.tpl ${GEN_DIR}/templates/expr_unary_auto_hh.tpl ${GEN_DIR}/templates/expr_unary_auto_cc.tpl 
    COMMENT "Autogenerating based on niels.json"
    VERBATIM)

set_source_files_properties(${SRC_DIR}/nls/ast/expr_unary_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${SRC_DIR}/nls/ast/expr_unary_auto.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(${SRC_DIR}/nls/ast/expr_binary_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${SRC_DIR}/nls/ast/expr_binary_auto.cc PROPERTIES GENERATED TRUE)

add_executable(niels
    ${HDR}
    ${SRC}
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
target_link_libraries(niels bh bhc)
