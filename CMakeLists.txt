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

LINK_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/lib/gcc
        /usr/lib
        /usr/local/lib
        ${CMAKE_DL_LIBS}
)

SET( SRCS
    ./src/main.cpp
    ./src/glad.c
    ./src/gl_program.cpp
    ./src/callback.cpp
    ./src/world.cpp
    ./src/physics.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3 X11 GL dl pthread)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3  pthread)
