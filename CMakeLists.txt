cmake_minimum_required(VERSION 4.0)
project(Project)

set(CMAKE_CXX_STANDARD 20)

# Set SDL3 Include and Lib Paths
if (WIN32)
    set(SDL3_PATH "C:/Dev/sdl3" CACHE PATH "Path to SDL3 root directory")
    set(SDL3_IMAGE_PATH "C:/Dev/sdl3_image" CACHE PATH "Path to SDL3_image root directory")
    set(SDL3_TTF_Path "C:/Dev/sdl3_ttf" CACHE PATH "Path to SDL3_ttf root directory")
    set(SDL3_MIXER_PATH "C:/Dev/sdl3_mixer 3.1.0" CACHE PATH "Path to SDL3_mixer root directory")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${SDL3_TTF_Path}/include
            ${SDL3_MIXER_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
            ${SDL3_TTF_Path}/lib
            ${SDL3_MIXER_PATH}/lib
    )
endif ()

add_executable(Project src/main.cpp
        src/Game.cpp
        src/TextureManager.h
        src/TextureManager.cpp
        src/Map.cpp
        src/gameObject.cpp
        src/utils/Vector2D.cpp
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.cpp
        src/ecs/World.cpp
        src/utils/CollisionObservers.cpp
        src/vendor/tinyxml2.cpp
        src/manager/AssetManager.cpp
        src/manager/Scene.cpp)


target_link_libraries(Project
        SDL3
        SDL3_image
        SDL3_ttf
        SDL3_mixer
)