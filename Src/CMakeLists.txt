message(STATUS "Config app current dir is ${CMAKE_CURRENT_SOURCE_DIR}")
FILE(GLOB SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/window/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/window/DiagramWindow/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/window/ProduceWithEditWindow/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/function/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/function/AnalysisCode/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/function/UnitTestCodeProduce/*.cpp") 

add_executable(
    ${PROJECT_NAME}
    #${QRC_FILES}
    ${SRC_FILES}
)

clang_target_link(${PROJECT_NAME} PRIVATE ${LOCAL_LINK_LIB})