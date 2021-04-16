message(STATUS "Config unittest current dir is ${CMAKE_CURRENT_SOURCE_DIR}")

#find_package(GTest QUIET CONFIG)
find_package(Catch2 QUIET CONFIG)
if(Catch2_FOUND)
    list(APPEND LOCAL_LINK_LIB Catch2::Catch2)
else(Catch2_FOUND)
    include_directories(${CMAKE_SOURCE_DIR}/Lib/catch2)
endif(Catch2_FOUND)

FILE(GLOB AUTESTSRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TrainingCode/*.cxx"
    "${CMAKE_SOURCE_DIR}/Src/function/*.cpp"
    "${CMAKE_SOURCE_DIR}/Src/function/AnalysisCode/*.cpp")

## 指定测试用的代码路径
set(TESTCODE_FULLPATH "${CMAKE_CURRENT_SOURCE_DIR}/TrainingCode/")
set(COMPILE_COMMANDS_FILEPATH ${CMAKE_BINARY_DIR})
add_definitions(-D_CRT_SECURE_NO_WARNINGS 
                -DTESTCODE_FULLPATH="${TESTCODE_FULLPATH}" 
                -DCOMPILE_COMMANDS_FILEPATH="${COMPILE_COMMANDS_FILEPATH}")

set(PROJECTNAME automatedtools_unittests)

add_executable(${PROJECTNAME} ${AUTESTSRC})
clang_target_link(${PROJECTNAME} PRIVATE ${LOCAL_LINK_LIB})

#add_subdirectory(TrainingCode)

#----------------------------------------------------
#******************** 工具可以打印出 target 的属性 ************************************
# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

function(print_properties)
    message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction(print_properties)

function(print_target_properties tgt)
    if(NOT TARGET ${tgt})
      message("There is no target named '${tgt}'")
      return()
    endif()

    foreach (prop ${CMAKE_PROPERTY_LIST})
        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
        #message ("Checking ${prop}")
        get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
        if (propval)
            get_target_property(propval ${tgt} ${prop})
            message ("caolei ${tgt} ${prop} = ${propval}")
        endif()
    endforeach(prop)
endfunction(print_target_properties)

#print_target_properties(Catch2::Catch2)
#******************** 工具可以打印出 target 的属性 ************************************#