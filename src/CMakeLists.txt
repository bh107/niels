cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BOHRIUM_LIB "$ENV{HOME}/.local/lib" CACHE STRING "Bohrium lib path")
set(BOHRIUM_INC "$ENV{HOME}/.local/include" CACHE STRING "Bohrium inc path")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/nls)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/nls/ast)
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
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/stmt.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/visitor.hh")
set(HDR "${HDR};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/evaluator.hh")

set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/niels.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/utils.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/driver.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/symboltable.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/node.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/expr.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/stmt.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/visitor.cc")
set(SRC "${SRC};${CMAKE_CURRENT_SOURCE_DIR}/nls/ast/evaluator.cc")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ast_unary_ops_auto.hh ${CMAKE_CURRENT_SOURCE_DIR}/ast_vtype_auto.hh ${CMAKE_CURRENT_SOURCE_DIR}/ast_vtype_auto.cc ${CMAKE_CURRENT_SOURCE_DIR}/ast_binary_ops_auto.hh ${CMAKE_CURRENT_SOURCE_DIR}/ast_binary_ops_auto.cc ${CMAKE_CURRENT_SOURCE_DIR}/ast_unary_ops_auto.cc

    COMMAND ./niels.py > niels.json
    COMMAND ./gen.py --generator=value_auto.hh > ${CMAKE_CURRENT_SOURCE_DIR}/value_auto.hh
    COMMAND ./gen.py --generator=value_auto.cc > ${CMAKE_CURRENT_SOURCE_DIR}/value_auto.cc
    COMMAND ./gen.py --generator=expr_unary_auto.hh > ${CMAKE_CURRENT_SOURCE_DIR}/expr_unary_auto.hh
    COMMAND ./gen.py --generator=expr_unary_auto.cc > ${CMAKE_CURRENT_SOURCE_DIR}/expr_unary_auto.cc
    COMMAND ./gen.py --generator=expr_binary_auto.hh > ${CMAKE_CURRENT_SOURCE_DIR}/expr_binary_auto.hh
    COMMAND ./gen.py --generator=expr_binary_auto.cc > ${CMAKE_CURRENT_SOURCE_DIR}/expr_binary_auto.cc

    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/autogen
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/autogen/niels.py ${CMAKE_CURRENT_SOURCE_DIR}/autogen/niels.json ${CMAKE_CURRENT_SOURCE_DIR}/autogen/gen.py ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/ast_binary_ops_cc.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/ast_unary_ops_hh.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/ast_unary_ops_cc.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/ast_binary_ops_hh.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/ast_vtype_cc.tpl ${CMAKE_CURRENT_SOURCE_DIR}/autogen/templates/ast_vtype_hh.tpl
    COMMENT "Autogenerating based on niels.json"
    VERBATIM)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/value_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/value_auto.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/expr_unary_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/expr_unary_auto.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/expr_binary_auto.hh PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/expr_binary_auto.cc PROPERTIES GENERATED TRUE)

add_executable(niels
    ${HDR}
    ${SRC}
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
target_link_libraries(niels bh bhc)
