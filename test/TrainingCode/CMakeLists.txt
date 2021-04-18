message(STATUS "Config unittest current dir is ${CMAKE_CURRENT_SOURCE_DIR}")

FILE(GLOB AUTESTSRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/UnitTestCodeProduceTemplate.cxx"
    #"${CMAKE_SOURCE_DIR}/Src/function/*.cpp"
    )


set(COMPILE_COMMANDS_FILEPATH ${CMAKE_BINARY_DIR})
add_definitions(-D_CRT_SECURE_NO_WARNINGS 
                -DCOMPILE_COMMANDS_FILEPATH="${COMPILE_COMMANDS_FILEPATH}")

set(PROJECTNAME testcode)

add_executable(${PROJECTNAME} ${AUTESTSRC})
target_link_libraries(${PROJECTNAME})
