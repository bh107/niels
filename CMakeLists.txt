cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BOHRIUM_LIB "$ENV{HOME}/.local/lib" CACHE STRING "Bohrium lib path")
set(BOHRIUM_INC "$ENV{HOME}/.local/include" CACHE STRING "Bohrium inc path")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gen")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
INCLUDE_DIRECTORIES(${SRC_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES("${BOHRIUM_INC}/")
INCLUDE_DIRECTORIES("${BOHRIUM_INC}/bohrium")
LINK_DIRECTORIES("${BOHRIUM_LIB}")

BISON_TARGET(NielsParser "${SRC_DIR}/parser.yy" ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(NielsScanner "${SRC_DIR}/scanner.ll" ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

set(GENERATED_SOURCE_FILES
    ${SRC_DIR}/nls/ast/nodes.hh
    ${SRC_DIR}/nls/ast/nodes.cc
    ${SRC_DIR}/nls/ast/exception.hh
    ${SRC_DIR}/nls/ast/visitor/visitor_visit_auto_hh.inc
    ${SRC_DIR}/nls/ast/visitor/evaluator_visit_auto_hh.inc
    ${SRC_DIR}/nls/ast/visitor/evaluator_visit_auto.cc
)

set(SOURCE_FILES
    ${SRC_DIR}/niels.cc
    ${SRC_DIR}/nls/nls.hh
    ${SRC_DIR}/nls/variant.hh
    ${SRC_DIR}/nls/utils.hh
    ${SRC_DIR}/nls/utils.cc
    ${SRC_DIR}/nls/driver.hh
    ${SRC_DIR}/nls/driver.cc
    ${SRC_DIR}/nls/ast/node.hh
    ${SRC_DIR}/nls/ast/node.cc
    ${SRC_DIR}/nls/symbol_table.hh
    ${SRC_DIR}/nls/symbol_table.cc
    ${SRC_DIR}/nls/ast/visitor/visitor.hh
    ${SRC_DIR}/nls/ast/visitor/visitor.cc
    ${SRC_DIR}/nls/ast/visitor/evaluator.hh
    ${SRC_DIR}/nls/ast/visitor/evaluator.cc
    ${GENERATED_SOURCE_FILES}
)

set(GENERATOR_TEMPLATES
    ${GEN_DIR}/templates/nodes_hh.tpl
    ${GEN_DIR}/templates/nodes_cc.tpl
    ${GEN_DIR}/templates/visitor_visit_auto_hh_inc.tpl
    ${GEN_DIR}/templates/evaluator_visit_auto_hh_inc.tpl
    ${GEN_DIR}/templates/evaluator_visit_auto_cc.tpl
)

set(GENERATOR_FILES
    ${GEN_DIR}/ast.py
    ${GEN_DIR}/ast.yaml
)

add_custom_command(OUTPUT ${GENERATED_SOURCE_FILES}
    COMMAND ./ast.py --output_root ${SRC_DIR}

    WORKING_DIRECTORY ${GEN_DIR}
    DEPENDS  ${GENERATOR_FILES} ${GENERATOR_TEMPLATES}
    COMMENT "Generating AST nodes and visitor boiler-plate."
    VERBATIM)

set_source_files_properties(${GENERATED_SOURCE_FILES} PROPERTIES GENERATED TRUE)

add_executable(niels
    ${SOURCE_FILES}
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
target_link_libraries(niels bh bhc)
