project(ProjectUnitTest CXX)

set(AUTESTSRC 
    main.cpp 
    LoopConvert.cpp
   )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

enable_testing()

llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})

set(CLANG_LINK_LIBRARIES
  libclang
  clangTooling
  clangARCMigrate
  clangAST
  clangASTMatchers
  clangAnalysis
  clangBasic
  clangCodeGen
  clangDriver
  clangEdit
  clangFrontend
  clangFrontendTool
  clangLex
  clangParse
  clangRewrite
  clangRewriteFrontend
  clangSema
  clangSerialization
  clangCodeGen
)

function(clang_target_link target type)
  if (CLANG_LINK_CLANG_DYLIB)
    target_link_libraries(${target} ${type} clang_shared)
  else()
    target_link_libraries(${target} ${type} ${ARGN})
  endif()
endfunction()

add_executable(${PROJECT_NAME} ${HEAD_FILES} ${AUTESTSRC})

clang_target_link(${PROJECT_NAME} PRIVATE ${CLANG_LINK_LIBRARIES})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})   