project(ProjectUnitTest CXX)

FILE(GLOB AUTESTSRC 
    "./*.cpp" 
    "./*.cxx" 
    "./Tools/*.hpp"
    "../Src/function/*.cpp"
    "../Src/function/AnalysisCode/*.cpp") 

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

enable_testing()

add_executable(${PROJECT_NAME} ${HEAD_FILES} ${AUTESTSRC})

clang_target_link(${PROJECT_NAME} PRIVATE ${linkLib} ${CLANG_LINK_LIBRARIES})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})