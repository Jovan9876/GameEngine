//
// Created by jovan on 2025-09-30.
//

#ifndef PROJECT_COLLISION_H
#define PROJECT_COLLISION_H
#include <SDL3/SDL_rect.h>

#include "Component.h"

class Collision {
    public:
        // Axis-aligned bounding box
        static bool AABB(const SDL_FRect& rectA, const SDL_FRect& rectB);
        static bool AABB(const Collider& colA, const Collider& colB);
};

#endif //PROJECT_COLLISION_H