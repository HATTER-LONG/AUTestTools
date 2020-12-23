cmake_minimum_required(VERSION 3.5)

project(Automated_UnitTest_Tools LANGUAGES CXX)

# 1. QT 以及 C++ 环境相关配置
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-Wall -pedantic)

find_package(Threads)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
set(QT5_LINK_LIBRARY Qt5::Widgets Qt5::Core Qt5::Gui)

## QT 资源文件添加
file(GLOB_RECURSE QRC_SOURCE_FILES "./*.qrc")
set(QRC_SOURCE_FILES image.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

# 2. Clang 库配置 提供库 Target 以及头文件路径标识 ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS}
find_package(Clang REQUIRED CONFIG)
set(CLANG_INCLUDE_DIR1 ${CLANG_INSTALL_PREFIX}/include/c++/v1/)
set(CLANG_INCLUDE_DIR2 ${CLANG_INSTALL_PREFIX}/lib/clang/11.0.0/include/)
set(CLANG_ANALYSIS_ARGS_APPEND "-isystem ${CLANG_INCLUDE_DIR1} -isystem ${CLANG_INCLUDE_DIR2} -stdlib=libc++")
set(LocalLibClang_DEFINITIONS  -fno-rtti 
    -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS 
    -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
    -DCLANG_ANALYSIS_ARGS_APPEND="${CLANG_ANALYSIS_ARGS_APPEND}")
add_definitions(${LocalLibClang_DEFINITIONS})

set(CLANG_LINK_LIBRARIES clangTooling)

# 3. 添加本地库与头文件搜索路径
set(LOCAL_LINK_LIB ${LocalLib} ${QT5_LINK_LIBRARY}  ${CLANG_LINK_LIBRARIES} pthread)

find_package(spdlog QUIET CONFIG)
if(spdlog_FOUND)
    list(APPEND LOCAL_LINK_LIB spdlog::spdlog)
elseif(spdlog_FOUND)
    ##通过通过静态链接使用 spdlog 库
    add_definitions(-DSPDLOG_COMPILED_LIB)
    add_subdirectory(Lib)
    include_directories(Lib/spdlog)
endif(spdlog_FOUND)
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
