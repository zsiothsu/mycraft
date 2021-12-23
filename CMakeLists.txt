cmake_minimum_required(VERSION 3.21)
project(final_cpp)

set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/inc/GLFW
        ${PROJECT_SOURCE_DIR}/inc/glad
        ${PROJECT_SOURCE_DIR}/inc/KHR
        ${PROJECT_SOURCE_DIR}/inc/stb

        /usr/include
        /user/local/include
)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    LINK_DIRECTORIES(
            ${PROJECT_SOURCE_DIR}/lib/gcc
            /usr/lib
            /usr/local/lib
            ${CMAKE_DL_LIBS}
    )
    ADD_DEFINITIONS(-DSYS_LINUX)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    LINK_DIRECTORIES(
            ${PROJECT_SOURCE_DIR}/lib/mingw
            ${CMAKE_DL_LIBS}
    )
    ADD_DEFINITIONS(-DSYS_WINDOWS)
ENDIF()

SET( SRCS
    ./src/main.cpp
    ./src/glad.c
    ./src/gl_program.cpp
    ./src/callback.cpp
    ./src/world.cpp
    ./src/physics.cpp
    ./src/perlin_noise.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3 X11 GL dl pthread)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
ENDIF()