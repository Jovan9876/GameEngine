//
// Created by jovan on 2025-09-09.
//

#ifndef PROJECT_TEXTUREMANAGER_H
#define PROJECT_TEXTUREMANAGER_H

#include <unordered_map>
#include <SDL3_image/SDL_image.h>
#include <string>

class TextureManager {
    static std::unordered_map<std::string, SDL_Texture *> textures;

public:
    static SDL_Texture *load(const char *path);

    // Floating point Rect = FRect
    static void draw(SDL_Texture *texture, SDL_FRect src, SDL_FRect dst);

    static void clean();
};

#endif //PROJECT_TEXTUREMANAGER_H
