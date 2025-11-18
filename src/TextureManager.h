//
// Created by jovan on 2025-09-09.
//

#ifndef PROJECT_TEXTUREMANAGER_H
#define PROJECT_TEXTUREMANAGER_H

#include <unordered_map>
#include <SDL3_image/SDL_image.h>
#include <string>

#include "Component.h"

class TextureManager {
    static std::unordered_map<std::string, SDL_Texture *> textures;

public:
    static SDL_Texture *load(const char *path);

    static void loadLabel(Label& label);
    static void updateLabel(Label& label);

    // Floating point Rect = FRect
    static void draw(SDL_Texture *texture, const SDL_FRect* src, const SDL_FRect* dst);

    static void clean();
};

#endif //PROJECT_TEXTUREMANAGER_H
