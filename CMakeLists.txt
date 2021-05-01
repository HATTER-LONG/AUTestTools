cmake_minimum_required(VERSION 3.5)

project(Automated_UnitTest_Tools LANGUAGES CXX)

# 1. C++ 环境相关配置
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-Wall -pedantic)

if(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    add_definitions(-fno-limit-debug-info)
endif()

find_package(Threads)

if(NOT DEFINED ENV{CPLUS_INCLUDE_PATH})
    Message(FATAL_ERROR "Please set environment variables CPLUS_INCLUDE_PATH for Clang code analysis")
endif()

# 2. Clang 库配置 提供库 Target 以及头文件路径标识 ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS}
find_package(Clang REQUIRED CONFIG)
# TODO: 增加判断逻辑是否由一些 clang 头文件，或者使用环境变量来获取

set(LocalLibClang_DEFINITIONS  -fno-rtti 
    -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS 
    -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)
add_definitions(${LocalLibClang_DEFINITIONS})

set(CLANG_LINK_LIBRARIES clangTooling)

# 3. 添加本地库与头文件搜索路径
set(LOCAL_LINK_LIB ${CLANG_LINK_LIBRARIES} pthread)

find_package(spdlog QUIET CONFIG)
if(spdlog_FOUND)
    list(APPEND LOCAL_LINK_LIB spdlog::spdlog)
else()
    include_directories(Lib/spdlog)
endif()

find_package(nlohmann_json QUIET CONFIG)
if(nlohmann_json_FOUND)
    list(APPEND LOCAL_LINK_LIB nlohmann_json::nlohmann_json)
else()
    include_directories(Lib/nlohmann)
endif()

## 指定头文件、库查找路径
include_directories(Include)
link_directories(Lib)

message(STATUS "LOCAL_LINK_LIB = ${LOCAL_LINK_LIB}")

function(clang_target_link target type)
  if (CLANG_LINK_CLANG_DYLIB)
    target_link_libraries(${target} ${type} clang_shared)
  else()
    target_link_libraries(${target} ${type} ${ARGN})
  endif()
endfunction()

add_subdirectory(Src)
add_subdirectory(test)
