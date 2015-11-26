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
    ${SRC_DIR}/nls/ast/abs.hh
    ${SRC_DIR}/nls/ast/abs.cc
    ${SRC_DIR}/nls/ast/accessor.hh
    ${SRC_DIR}/nls/ast/accessor.cc
    ${SRC_DIR}/nls/ast/acos.hh
    ${SRC_DIR}/nls/ast/acos.cc
    ${SRC_DIR}/nls/ast/acosh.hh
    ${SRC_DIR}/nls/ast/acosh.cc
    ${SRC_DIR}/nls/ast/add.hh
    ${SRC_DIR}/nls/ast/add.cc
    ${SRC_DIR}/nls/ast/alias.hh
    ${SRC_DIR}/nls/ast/alias.cc
    ${SRC_DIR}/nls/ast/and.hh
    ${SRC_DIR}/nls/ast/and.cc
    ${SRC_DIR}/nls/ast/anon.hh
    ${SRC_DIR}/nls/ast/anon.cc
    ${SRC_DIR}/nls/ast/arg_list.hh
    ${SRC_DIR}/nls/ast/arg_list.cc
    ${SRC_DIR}/nls/ast/as.hh
    ${SRC_DIR}/nls/ast/as.cc
    ${SRC_DIR}/nls/ast/asin.hh
    ${SRC_DIR}/nls/ast/asin.cc
    ${SRC_DIR}/nls/ast/asinh.hh
    ${SRC_DIR}/nls/ast/asinh.cc
    ${SRC_DIR}/nls/ast/assign.hh
    ${SRC_DIR}/nls/ast/assign.cc
    ${SRC_DIR}/nls/ast/atan.hh
    ${SRC_DIR}/nls/ast/atan.cc
    ${SRC_DIR}/nls/ast/atanh.hh
    ${SRC_DIR}/nls/ast/atanh.cc
    ${SRC_DIR}/nls/ast/attr.hh
    ${SRC_DIR}/nls/ast/attr.cc
    ${SRC_DIR}/nls/ast/attr_list.hh
    ${SRC_DIR}/nls/ast/attr_list.cc
    ${SRC_DIR}/nls/ast/block.hh
    ${SRC_DIR}/nls/ast/block.cc
    ${SRC_DIR}/nls/ast/bul.hh
    ${SRC_DIR}/nls/ast/bul.cc
    ${SRC_DIR}/nls/ast/bw_and.hh
    ${SRC_DIR}/nls/ast/bw_and.cc
    ${SRC_DIR}/nls/ast/bw_lshift.hh
    ${SRC_DIR}/nls/ast/bw_lshift.cc
    ${SRC_DIR}/nls/ast/bw_not.hh
    ${SRC_DIR}/nls/ast/bw_not.cc
    ${SRC_DIR}/nls/ast/bw_or.hh
    ${SRC_DIR}/nls/ast/bw_or.cc
    ${SRC_DIR}/nls/ast/bw_rshift.hh
    ${SRC_DIR}/nls/ast/bw_rshift.cc
    ${SRC_DIR}/nls/ast/bw_xor.hh
    ${SRC_DIR}/nls/ast/bw_xor.cc
    ${SRC_DIR}/nls/ast/call.hh
    ${SRC_DIR}/nls/ast/call.cc
    ${SRC_DIR}/nls/ast/case_list.hh
    ${SRC_DIR}/nls/ast/case_list.cc
    ${SRC_DIR}/nls/ast/ceil.hh
    ${SRC_DIR}/nls/ast/ceil.cc
    ${SRC_DIR}/nls/ast/comment.hh
    ${SRC_DIR}/nls/ast/comment.cc
    ${SRC_DIR}/nls/ast/cos.hh
    ${SRC_DIR}/nls/ast/cos.cc
    ${SRC_DIR}/nls/ast/div.hh
    ${SRC_DIR}/nls/ast/div.cc
    ${SRC_DIR}/nls/ast/empty.hh
    ${SRC_DIR}/nls/ast/empty.cc
    ${SRC_DIR}/nls/ast/equal.hh
    ${SRC_DIR}/nls/ast/equal.cc
    ${SRC_DIR}/nls/ast/exp.hh
    ${SRC_DIR}/nls/ast/exp.cc
    ${SRC_DIR}/nls/ast/floor.hh
    ${SRC_DIR}/nls/ast/floor.cc
    ${SRC_DIR}/nls/ast/function_body.hh
    ${SRC_DIR}/nls/ast/function_body.cc
    ${SRC_DIR}/nls/ast/function_def.hh
    ${SRC_DIR}/nls/ast/function_def.cc
    ${SRC_DIR}/nls/ast/gthan.hh
    ${SRC_DIR}/nls/ast/gthan.cc
    ${SRC_DIR}/nls/ast/gthan_equal.hh
    ${SRC_DIR}/nls/ast/gthan_equal.cc
    ${SRC_DIR}/nls/ast/i32.hh
    ${SRC_DIR}/nls/ast/i32.cc
    ${SRC_DIR}/nls/ast/i64.hh
    ${SRC_DIR}/nls/ast/i64.cc
    ${SRC_DIR}/nls/ast/ident.hh
    ${SRC_DIR}/nls/ast/ident.cc
    ${SRC_DIR}/nls/ast/import.hh
    ${SRC_DIR}/nls/ast/import.cc
    ${SRC_DIR}/nls/ast/is.hh
    ${SRC_DIR}/nls/ast/is.cc
    ${SRC_DIR}/nls/ast/isinf.hh
    ${SRC_DIR}/nls/ast/isinf.cc
    ${SRC_DIR}/nls/ast/isnan.hh
    ${SRC_DIR}/nls/ast/isnan.cc
    ${SRC_DIR}/nls/ast/log.hh
    ${SRC_DIR}/nls/ast/log.cc
    ${SRC_DIR}/nls/ast/log10.hh
    ${SRC_DIR}/nls/ast/log10.cc
    ${SRC_DIR}/nls/ast/log2.hh
    ${SRC_DIR}/nls/ast/log2.cc
    ${SRC_DIR}/nls/ast/lthan.hh
    ${SRC_DIR}/nls/ast/lthan.cc
    ${SRC_DIR}/nls/ast/lthan_equal.hh
    ${SRC_DIR}/nls/ast/lthan_equal.cc
    ${SRC_DIR}/nls/ast/max.hh
    ${SRC_DIR}/nls/ast/max.cc
    ${SRC_DIR}/nls/ast/min.hh
    ${SRC_DIR}/nls/ast/min.cc
    ${SRC_DIR}/nls/ast/mod.hh
    ${SRC_DIR}/nls/ast/mod.cc
    ${SRC_DIR}/nls/ast/module.hh
    ${SRC_DIR}/nls/ast/module.cc
    ${SRC_DIR}/nls/ast/mul.hh
    ${SRC_DIR}/nls/ast/mul.cc
    ${SRC_DIR}/nls/ast/new.hh
    ${SRC_DIR}/nls/ast/new.cc
    ${SRC_DIR}/nls/ast/noop.hh
    ${SRC_DIR}/nls/ast/noop.cc
    ${SRC_DIR}/nls/ast/not.hh
    ${SRC_DIR}/nls/ast/not.cc
    ${SRC_DIR}/nls/ast/not_equal.hh
    ${SRC_DIR}/nls/ast/not_equal.cc
    ${SRC_DIR}/nls/ast/or.hh
    ${SRC_DIR}/nls/ast/or.cc
    ${SRC_DIR}/nls/ast/otherwise.hh
    ${SRC_DIR}/nls/ast/otherwise.cc
    ${SRC_DIR}/nls/ast/param.hh
    ${SRC_DIR}/nls/ast/param.cc
    ${SRC_DIR}/nls/ast/param_list.hh
    ${SRC_DIR}/nls/ast/param_list.cc
    ${SRC_DIR}/nls/ast/pow.hh
    ${SRC_DIR}/nls/ast/pow.cc
    ${SRC_DIR}/nls/ast/query.hh
    ${SRC_DIR}/nls/ast/query.cc
    ${SRC_DIR}/nls/ast/r32.hh
    ${SRC_DIR}/nls/ast/r32.cc
    ${SRC_DIR}/nls/ast/r64.hh
    ${SRC_DIR}/nls/ast/r64.cc
    ${SRC_DIR}/nls/ast/range.hh
    ${SRC_DIR}/nls/ast/range.cc
    ${SRC_DIR}/nls/ast/record.hh
    ${SRC_DIR}/nls/ast/record.cc
    ${SRC_DIR}/nls/ast/record_def.hh
    ${SRC_DIR}/nls/ast/record_def.cc
    ${SRC_DIR}/nls/ast/return.hh
    ${SRC_DIR}/nls/ast/return.cc
    ${SRC_DIR}/nls/ast/shape.hh
    ${SRC_DIR}/nls/ast/shape.cc
    ${SRC_DIR}/nls/ast/sin.hh
    ${SRC_DIR}/nls/ast/sin.cc
    ${SRC_DIR}/nls/ast/sqrt.hh
    ${SRC_DIR}/nls/ast/sqrt.cc
    ${SRC_DIR}/nls/ast/stmt.hh
    ${SRC_DIR}/nls/ast/stmt.cc
    ${SRC_DIR}/nls/ast/stmt_list.hh
    ${SRC_DIR}/nls/ast/stmt_list.cc
    ${SRC_DIR}/nls/ast/str.hh
    ${SRC_DIR}/nls/ast/str.cc
    ${SRC_DIR}/nls/ast/sub.hh
    ${SRC_DIR}/nls/ast/sub.cc
    ${SRC_DIR}/nls/ast/tan.hh
    ${SRC_DIR}/nls/ast/tan.cc
    ${SRC_DIR}/nls/ast/trunc.hh
    ${SRC_DIR}/nls/ast/trunc.cc
    ${SRC_DIR}/nls/ast/when.hh
    ${SRC_DIR}/nls/ast/when.cc
    ${SRC_DIR}/nls/ast/while.hh
    ${SRC_DIR}/nls/ast/while.cc
    ${SRC_DIR}/nls/ast/xor.hh
    ${SRC_DIR}/nls/ast/xor.cc
    ${SRC_DIR}/nls/ast/nodes.hh
    ${SRC_DIR}/nls/ast/visitor/visitor_visit_auto_hh.inc
    ${SRC_DIR}/nls/ast/visitor/evaluator_visit_auto_hh.inc
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
    ${GENERATED_SOURCE_FILES}
)

set(GENERATOR_TEMPLATES
    ${GEN_DIR}/templates/nodes_hh.tpl
    ${GEN_DIR}/templates/node_skeleton_hh.tpl
    ${GEN_DIR}/templates/node_skeleton_cc.tpl
    ${GEN_DIR}/templates/visitor_visit_auto_hh_inc.tpl
    ${GEN_DIR}/templates/evaluator_visit_auto_hh_inc.tpl
    ${GEN_DIR}/templates/evaluator_visit_unary_auto_cc.tpl
    ${GEN_DIR}/templates/evaluator_visit_binary_auto_cc.tpl
)

set(GENERATOR_FILES
    ${GEN_DIR}/ast.py
    ${GEN_DIR}/ast.yaml
    ${GEN_DIR}/gen.py
    ${GEN_DIR}/niels.json
)

add_custom_command(OUTPUT ${GENERATED_SOURCE_FILES}
    COMMAND ./ast.py --output_root ${SRC_DIR}
    WORKING_DIRECTORY ${GEN_DIR}
    DEPENDS  ${GENERATOR_FILES} ${GENERATOR_TEMPLATES}
    COMMENT "Generating AST nodes and visitor boiler-plate."
    VERBATIM)
#    COMMAND ./niels.py > niels.json
#    COMMAND ./gen.py --generator=expr_unary_auto.hh > ${SRC_DIR}/nls/ast/expr_unary_auto.hh
#    COMMAND ./gen.py --generator=expr_unary_auto.cc > ${SRC_DIR}/nls/ast/expr_unary_auto.cc
#    COMMAND ./gen.py --generator=expr_binary_auto.hh > ${SRC_DIR}/nls/ast/expr_binary_auto.hh
#    COMMAND ./gen.py --generator=expr_binary_auto.cc > ${SRC_DIR}/nls/ast/expr_binary_auto.cc

set_source_files_properties(${GENERATED_FILES} PROPERTIES GENERATED TRUE)

add_executable(niels
    ${SOURCE_FILES}
    ${BISON_NielsParser_OUTPUTS}
    ${FLEX_NielsScanner_OUTPUTS}
)
target_link_libraries(niels bh bhc)
