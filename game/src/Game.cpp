//
// Created by Jovan on 2025-09-24.
//

#include "Game.h"
#include <glad/glad.h>
#include <chrono>
#include <iostream>
#include <ostream>


using namespace std;

Game::Game() {}

Game::~Game() {
    destroy();
}

void Game::init(const char* title, int width, int height, bool fullscreen) {

    // Set opengl flags
    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
    }

    // Init SDL Library
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 1){
        std::cout << "Subsystem initialized..." << std::endl;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Create OpenGL capable window
        window = SDL_CreateWindow(title, width, height, flags);
        if (window){
            std::cout << "Window created..." << std::endl;
        }
        else{
            std::cout << "Window could not be created..." << std::endl;
            isRunning = false;
            return;
        }

        // Create opengl context
        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }

        //set current context
        SDL_GL_MakeCurrent(window, glContext);

        //check if glad is loaded
        if (!gladLoadGL()) {
            std::cerr << "Failed to load OpenGL functions\n";
            isRunning = false;
            return;
        }

        compileShader();      // create & link your shaders
        createVAOAndVBO();    // setup your triangle data

        // Enable depth testing (for 3D)
        glEnable(GL_DEPTH_TEST);

        isRunning = true;
    }
    else{
        isRunning = false;
    }
}

void Game::handleEvents() {
    // SDL listens to the OS for input events added to a queue
    SDL_Event event;

    // Check for next event, if available it will remove from the queue and store in event
    SDL_PollEvent(&event);

    switch (event.type){
    case SDL_EVENT_QUIT: // Triggers on window close
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update(float deltaTime) {
    frameCount++;
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Clear the color buffer (and depth if you use 3D later)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program
    glUseProgram(shaderProgram);

    // Bind VAO and draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap the buffers
    SDL_GL_SwapWindow(window);
}

void Game::destroy() {
    //destroy context
    SDL_GL_DestroyContext(glContext);

    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "GameDestroyed" << std::endl;
}

void Game::createVAOAndVBO() {

    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Game::compileShader() {
    const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); //orange
    }
    )";

    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader compilation failed:\n" << infoLog << std::endl;
    }

    // Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader compilation failed:\n" << infoLog << std::endl;
    }

    // Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program linking failed:\n" << infoLog << std::endl;
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}