cmake_minimum_required(VERSION 3.5)

project(Automated_UnitTest LANGUAGES CXX)

# set (CMAKE_PREFIX_PATH "/home/xxx/Qt5.13.2/5.13.2/gcc_64")
# or
# CMAKE_PREFIX_PATH=/home/xxx/Qt5.13.2/5.13.2/gcc_64
# 请检查环境变量CMAKE_PREFIX_PATH 是否新增 Qt 相关路径

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads) #查找 多线程库
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

find_package(Clang REQUIRED CONFIG)

#静态库的方式连接 spdlog
add_definitions(-DSPDLOG_COMPILED_LIB) 
#指定连结库的路径
link_directories(./Lib ${CLANG_LIBRARY_DIRS} ${LLVM_LIBRARY_DIRS}) 
#指定头文件查找路径
include_directories(Include ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})

file(GLOB_RECURSE QRC_SOURCE_FILES "./*.qrc")
set(QRC_SOURCE_FILES image.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})


#查找当前文件夹中的所有源代码文件，也可以通过Set命令将所有文件设置为一个变量
FILE(GLOB SRC_FILES 
    "./Src/*.cpp" 
    "./Src/window/*.cpp" 
    "./Src/window/DiagramWindow/*.cpp"
    "./Src/function/*.cpp"
    "./Sec/function/AnalysisCode/*.cpp") 
#同理，查找当前文件夹中所有的头文件
FILE(GLOB HEAD_FILES "Include/*") 
#同理，查找当前文件夹中所有的ui文件
FILE(GLOB UI_FILES "./Src/window/*.ui")

add_executable(
    ${PROJECT_NAME}
    ${QRC_FILES}
    ${SRC_FILES}
    ${HEAD_FILES}
)

set(linkLib Qt5::Widgets Qt5::Core Qt5::Gui  libclang)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
    list(APPEND linkLib spdlogd)
ELSE()
    list(APPEND linkLib spdlog)
ENDIF()

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

IF(WIN32)
    
elseif(UNIX)
    list(APPEND linkLib pthread)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
else()
    message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()

add_subdirectory(test)

#target_link_libraries(
 #   ${PROJECT_NAME} 
 #   ${linkLib}
#)
clang_target_link(${PROJECT_NAME} PRIVATE ${linkLib} ${CLANG_LINK_LIBRARIES})
