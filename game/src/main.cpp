#include <iostream>
#include "Game.h"

// Global variable
Game *game = nullptr;

int main() {

    const int FPS = 60;
    const int desiredFrameTime = 1000 / FPS; // 16ms per frame

    // Uint64 ticks;
    // int actualFrameTime;

    Uint64 lastTick = SDL_GetTicks();

    game = new Game();
    game->init("8552 Tutorial Engine", 800, 600, false);

    // Game loop
    while (game->running()) {
        // ticks = SDL_GetTicks(); // Time in milliseconds since we initialized SDL

        Uint64 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f; // Seconds
        lastTick = currentTick;

        game->handleEvents();
        game->update(deltaTime);
        game->render();

        // actualFrameTime = SDL_GetTicks() - ticks; // Elapsed time in milliseconds it took for the current frame

        int frameTime = SDL_GetTicks() - lastTick;

        // Frame limiter
        // if (desiredFrameTime > actualFrameTime) {
        //     SDL_Delay(desiredFrameTime - actualFrameTime);
        // }

        if (desiredFrameTime > frameTime) {
            SDL_Delay(desiredFrameTime - frameTime);
        }

    }

    delete game;

    return 0;
}