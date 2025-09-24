//
// Created by Jovan on 2025-09-24.
//

#ifndef GAMEENGINE_GAME_H
#define GAMEENGINE_GAME_H

#include <SDL3/SDL.h>

class Game {
public:
    Game();
    ~Game();

    // Types that SDL uses
    void init(const char* title, int width, int height, bool fullscreen);

    // Game Loop
    void handleEvents(); // Checks for input and system events
    void update(float deltaTime); // Handles game logic and changes to game state
    void render(); // Handles drawing to screen

    // Free rss's
    void destroy();

    bool running() { return isRunning; }


private:
    int frameCount = 0;
    bool isRunning = false;

    SDL_Window* window = nullptr;
    SDL_Event event;
    SDL_GLContext glContext = nullptr;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int shaderProgram;
    int colorLocation;

    void createVAOAndVBO();
    void compileShader();

    float vertices[9] = {
        0.0f,  0.5f, 0.0f,  // top vertex
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f   // bottom left
    };

};
#endif //GAMEENGINE_GAME_H
