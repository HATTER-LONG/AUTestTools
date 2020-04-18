project(ProjectUnitTest CXX)

set(AUTESTSRC 
    main.cpp 
   )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   
enable_testing()

add_executable(${PROJECT_NAME} ${HEAD_FILES} ${AUTESTSRC})

target_link_libraries(${PROJECT_NAME} PRIVATE ${linkLib})

add_test(NAME $${PROJECT_NAME} COMMAND ${PROJECT_NAME})   