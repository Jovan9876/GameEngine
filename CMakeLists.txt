cmake_minimum_required(VERSION 4.0)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)

# Set SDL3 Include and Lib Paths
if (WIN32)
    set(SDL3_PATH "C:/Dev/sdl3")
    set(GLAD_PATH "C:/Dev/glad")
    include_directories(
            ${SDL3_PATH}/include
            ${GLAD_PATH}/include
    )
    link_directories(
            ${SDL3_PATH}/lib
    )
endif ()

add_executable(GameEngine game/src/main.cpp
        engine/src/Engine.cpp
        game/src/Game.cpp
        game/src/glad.c
)

target_link_libraries(GameEngine
        SDL3
)


