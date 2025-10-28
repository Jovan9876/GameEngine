//
// Created by jovan on 2025-09-02.
//

#include "Game.h"
#include "Map.h"

#include <chrono>
#include <iostream>
#include <ostream>

#include "manager/AssetManager.h"


std::function<void(std::string)> Game::onSceneChangeRequest;

using namespace std;

Game::Game() {
}

Game::~Game() {
    destroy();
}

void Game::init(const char *title, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Init SDL Library
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 1) {
        std::cout << "Subsystem initialized..." << std::endl;
        window = SDL_CreateWindow(title, width, height, flags);
        if (window) {
            std::cout << "Window created..." << std::endl;
        }

        // Windows will be Direct3D (DirectX)
        // Mac will likely be Metal, OpenGL
        renderer = SDL_CreateRenderer(window, nullptr);

        if (renderer) {
            std::cout << "Renderer created..." << std::endl;
        } else {
            std::cout << "Renderer could not be created." << std::endl;
        }

        // Start with an initial color
        r = 255;
        g = 0;
        b = 0;
        a = 255;

        isRunning = true;
    } else {
        isRunning = false;
    }

    // Load assets
    AssetManager::loadAnimation("player", "../asset/animations/vampire_animations.xml");
    AssetManager::loadAnimation("enemy", "../asset/animations/bird_animations.xml");

    // Load scenes
    sceneManager.loadScene("level1", "../asset/map.tmx", width, height);
    sceneManager.loadScene("level2", "../asset/map2.tmx", width, height);

    // Start level 1
    sceneManager.changeSceneDeferred("level1");

    // Resolve scene callback
    onSceneChangeRequest = [this](std::string sceneName) {
        // Game state
        if (sceneManager.currentScene->getName() == "level2" && sceneName == "level2") {
            std::cout << "You Win!" << std::endl;
            isRunning = false;
            return;
        }

        if (sceneName == "gameover") {
            std::cout << "Game Over!" << std::endl;
            isRunning = false;
            return;
        }

        sceneManager.changeSceneDeferred(sceneName);
    };
}

void Game::handleEvents() {
    // SDL listens to the OS for input events added to a queue
    // SDL_Event event;


    // Check for next event, if available it will remove from the queue and store in event
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT: // Triggers on window close
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update(float dt) {
    frameCount++;
    sceneManager.update(dt, event);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Every frame the renderer is cleared with the draw color
    SDL_RenderClear(renderer);

    sceneManager.render();

    // Swap back buffer to screen
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    TextureManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "Game Destroyed" << endl;
}
