//
// Created by jovan on 2025-09-16.
//

#ifndef PROJECT_COMPONENT_H
#define PROJECT_COMPONENT_H
#include <functional>
#include <SDL3/SDL_render.h>
#include "Vector2D.h"
#include "AnimationClip.h"
#include <string>
#include <unordered_map>


struct Transform {
    Vector2D position{};
    float rotation{};
    float scale{};
    Vector2D oldPosition{};
};

// Direction and speed
struct Velocity {
    Vector2D direction{};
    float speed{};
};

struct Sprite {
    SDL_Texture *texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
};

struct Collider {
    std::string tag;
    SDL_FRect rect{};
};

struct Animation {
    std::unordered_map<std::string, AnimationClip> clips{};
    std::string currentClip{};
    float time{}; // Time accumulated for the current frame
    int currentFrame{}; // Index of the current frame in the clip
    float speed = 0.1f;
};

struct Camera {
    SDL_FRect view;
    float worldWidth{};
    float worldHeight{};
};

struct TimedSpawner {
    float spawnInterval{};
    std::function<void()> spawnCallback{};
    float timer{};
};

// Game state, might have multiple scenes
struct SceneState {
    int coinsCollected = 0;
};

struct PlayerTag{};
struct ProjectileTag{};

#endif //PROJECT_COMPONENT_H
