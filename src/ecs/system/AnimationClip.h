//
// Created by jovan on 2025-10-14.
//

#ifndef PROJECT_ANIMATIONCLIP_H
#define PROJECT_ANIMATIONCLIP_H
#include <vector>
#include <SDL3/SDL_rect.h>

struct AnimationClip {
    std::vector<SDL_FRect> frameIndices;
};

#endif //PROJECT_ANIMATIONCLIP_H