//
// Created by jovan on 2025-09-09.
//

#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char *path, int x, int y) : xPos(x), yPos(y), texture(TextureManager::load(path)) {
}

GameObject::~GameObject() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void GameObject::update(float deltaTime) {
    // Move gameObject one pixel on the x and y each frame
    // xPos++;
    // yPos++;

    xPos += moveSpeed * deltaTime;
    yPos += moveSpeed * deltaTime;

    // Where do we want to crop the texture from
    srcRect.x = srcRect.y = 0;

    // How much to crop
    srcRect.w = srcRect.h = 32;

    // Where to draw it
    dstRect.x = static_cast<float>(xPos);
    dstRect.y = static_cast<float>(yPos);

    // What sizes to render
    dstRect.h = srcRect.h;
    dstRect.w = srcRect.w;
}

void GameObject::draw() {
    TextureManager::draw(texture, &srcRect, &dstRect);
}
