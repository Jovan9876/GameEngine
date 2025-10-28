//
// Created by jovan on 2025-09-09.
//

#ifndef PROJECT_GAMEOBJECT_H
#define PROJECT_GAMEOBJECT_H

#include "Game.h"

class GameObject {
public:
    GameObject(const char *path, int x, int y);

    float moveSpeed = 100.0f;

    ~GameObject();

    void update(float deltaTime);

    void draw();

private:
    int xPos{}, yPos{};
    SDL_Texture *texture = nullptr;
    SDL_FRect srcRect{}, dstRect{};
};

#endif //PROJECT_GAMEOBJECT_H
