//
// Created by jerry on 2025-11-17.
//

#ifndef GAMEENGINE_RENDERUTILS_H
#define GAMEENGINE_RENDERUTILS_H
#include <SDL3/SDL_rect.h>


class RenderUtils {
public:
    //scaling from center pointa
    static SDL_FRect getScaledDest(const SDL_FRect& dest, float scale) {
        return SDL_FRect{
            dest.x + dest.w * (1 - scale) / 2,
            dest.y + dest.h * (1 - scale) / 2,
        dest.w * scale, dest.h * scale
        };
    }
};
#endif //GAMEENGINE_RENDERUTILS_H