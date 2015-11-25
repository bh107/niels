cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BOHRIUM_LIB "$ENV{HOME}/.local/lib" CACHE STRING "Bohrium lib path")
set(BOHRIUM_INC "$ENV{HOME}/.local/include" CACHE STRING "Bohrium inc path")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${BOHRIUM_INC}/")
include_directories("${BOHRIUM_INC}/bohrium")
link_directories("${BOHRIUM_LIB}")

BISON_TARGET(NielsParser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(NielsScanner scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/nls.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/value.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/utils.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/driver.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/symboltable.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/node.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/stmt.hh")

set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/niels.cc")

set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/utils.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/driver.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/symboltable.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/node.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/stmt.cc")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.hh ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.hh ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.cc ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.cc ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.cc ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.hh

    COMMAND ./niels.py > niels.json
    COMMAND ./gen.py --generator=expr_value_auto.hh > ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.hh
    COMMAND ./gen.py --generator=expr_value_auto.cc > ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.cc
    COMMAND ./gen.py --generator=expr_unary_auto.hh > ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.hh
    COMMAND ./gen.py --generator=expr_unary_auto.cc > ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.cc
    COMMAND ./gen.py --generator=expr_binary_auto.hh > ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.hh
    COMMAND ./gen.py --generator=expr_binary_auto.cc > ${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.cc

    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/autogen
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/expr_value_auto_hh.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/expr_binary_auto_hh.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/expr_unary_auto_cc.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/expr_binary_auto_cc.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/expr_value_auto_cc.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/expr_unary_auto_hh.tpl
    COMMENT "Autogenerating based on niels.json"
    VERBATIM)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_value_auto.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_unary_auto.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr_binary_auto.cc PROPERTIES GENERATED TRUE)

add_executable(niels
    ${HDR}
    ${SRC}
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
target_link_libraries(niels bh bhc)
