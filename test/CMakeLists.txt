project(ProjectUnitTest CXX)

set(AUTESTSRC 
    main.cpp 
   )

#find_package(LLVM REQUIRED CONFIG)
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#message("LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_DIRS}")
#message("LLVM_DEFINITIONS=${LLVM_DEFINITIONS}")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
enable_testing()

add_executable(${PROJECT_NAME} ${HEAD_FILES} ${AUTESTSRC})

target_link_libraries(${PROJECT_NAME} PRIVATE

${linkLib}
LoopConvertTooling

${clangAST}
${clangBasic}
${clangDriver}
${clangFormat}
${clangLex}
${clangParse}
${clangSema}
${clangFrontend}
${clangTooling}
${clangToolingCore}
${clangRewrite}
${clangRewriteFrontend}

${LLVMSupport}
)

add_test(NAME $${PROJECT_NAME} COMMAND ${PROJECT_NAME})   