message(STATUS "Config Lib current dir is ${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND LOCAL_LINK_LIB spdlog)
set(LOCAL_LINK_LIB
    ${LOCAL_LINK_LIB}
    PARENT_SCOPE)
