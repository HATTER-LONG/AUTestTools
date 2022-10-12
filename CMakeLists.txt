# Maybe you need change this /to/your/clang/path or just delete it to use
# default of cmake detect
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

cmake_minimum_required(VERSION 3.5)

# Turn C on because llvm14+ need this
project(Automated_UnitTest_Tools LANGUAGES C CXX)

# 1. C++ environment config
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "Set up ccache ...")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

add_definitions(-Wall -pedantic)
if(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES
                                       "RelWithDebInfo")
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-fno-limit-debug-info)
  endif()
endif()

find_package(Threads)

if(UNIX)
  if(NOT DEFINED ENV{CPLUS_INCLUDE_PATH})
    message(
      WARNING
        "Please set environment variables CPLUS_INCLUDE_PATH for Clang code analysis \n\texample:\n export CPLUS_INCLUDE_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/include/"
    )
  endif()
endif()

# 1. Clang 库配置 提供库 Target 以及头文件路径标识 ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS}
find_package(Clang REQUIRED CONFIG)

include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})
set(LocalLibClang_DEFINITIONS -fno-rtti -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS
                              -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)
add_definitions(${LocalLibClang_DEFINITIONS})

set(CLANG_LINK_LIBRARIES clangTooling)

# 1. 添加本地库与头文件搜索路径
set(LOCAL_LINK_LIB ${CLANG_LINK_LIBRARIES} pthread)

find_package(spdlog QUIET CONFIG)
if(spdlog_FOUND)
  list(APPEND LOCAL_LINK_LIB spdlog::spdlog)
else()
  message(STATUS "spdlog not found using <WorkSpace>/Lib/spdlog")
  include_directories(Lib/spdlog)
endif()

find_package(nlohmann_json QUIET CONFIG)
if(nlohmann_json_FOUND)
  list(APPEND LOCAL_LINK_LIB nlohmann_json::nlohmann_json)
else()
  message(STATUS "nlohmann_json not found using <WorkSpace>/Lib/json")
  include_directories(Lib/nlohmann)
endif()

# 指定头文件、库查找路径
include_directories(Include)
link_directories(Lib)

message(STATUS "LOCAL_LINK_LIB = ${LOCAL_LINK_LIB}")

function(clang_target_link target type)
  if(CLANG_LINK_CLANG_DYLIB)
    target_link_libraries(${target} ${type} clang_shared)
  else()
    target_link_libraries(${target} ${type} ${ARGN})
  endif()
endfunction()

add_subdirectory(Src)
add_subdirectory(test)
