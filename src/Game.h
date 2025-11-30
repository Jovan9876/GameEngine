//
// Created by jovan on 2025-09-02.
//

// #ifndef PROJECT_GAME_H
// #define PROJECT_GAME_H
//
// #endif //PROJECT_GAME_H

#pragma once
#include <SDL3/SDL.h>
#include <World.h>

#include "manager/AudioManager.h"
#include "manager/SceneManager.h"

class Game {
public:
    Game();
    ~Game();

    // Types that SDL uses
    void init(const char *title, int width, int height, bool fullscreen);

    // Game Loop
    void handleEvents(); // Checks for input and system events
    void update(float deltaTime); // Handles game logic and changes to game state
    void render(); // Handles drawing to screen

    // Free rss's
    void destroy();

    bool running() {
        return isRunning;
    }

    SDL_Renderer* renderer = nullptr;

    //Audio
    AudioManager audioManager;

    SceneManager sceneManager;
    static std::function<void(std::string)> onSceneChangeRequest;

private:
    int frameCount = 0;
    bool isRunning;

    SDL_Window *window = nullptr;

    SDL_Event event;

    // SDL color channels 8-bit unsigned
    // Range 0-255
    Uint8 r, g, b, a;
};
