cmake_minimum_required(VERSION 3.5)

project(Automated_UnitTest LANGUAGES CXX)

# set (CMAKE_PREFIX_PATH "/home/xxx/Qt5.13.2/5.13.2/gcc_64")
# or
# CMAKE_PREFIX_PATH=/home/xxx/Qt5.13.2/5.13.2/gcc_64
# 请检查环境变量CMAKE_PREFIX_PATH 是否新增 Qt 相关路径

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Threads) #查找 多线程库
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
#静态库的方式连接 spdlog
add_definitions(-DSPDLOG_COMPILED_LIB) 
#指定连结库的路径
link_directories(Lib) 

#指定头文件查找路径
include_directories(Include) 
#查找当前文件夹中的所有源代码文件，也可以通过Set命令将所有文件设置为一个变量
FILE(GLOB SRC_FILES "./*.cpp" "./Src/window/*.cpp") 
#同理，查找当前文件夹中所有的头文件
FILE(GLOB HEAD_FILES "./*.h" "Include/window/*.h") 
#同理，查找当前文件夹中所有的ui文件
FILE(GLOB UI_FILES "./Src/window/*.ui")

add_executable(
    ${PROJECT_NAME}
    ${SRC_FILES}
    ${HEAD_FILES}
)

set(linkLib Qt5::Widgets Qt5::Core Qt5::Gui)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
    list(APPEND linkLib spdlogd)
ELSE()
    list(APPEND linkLib spdlog)
ENDIF()

IF(WIN32)
list(APPEND linkLib clang)
elseif(UNIX)
    list(APPEND linkLib pthread)
else()
    message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()

target_link_libraries(
    ${PROJECT_NAME} 
    ${linkLib}
)
