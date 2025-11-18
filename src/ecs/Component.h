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

#include "SDL3_ttf/SDL_ttf.h"
#include "Entity.h"


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

//For UI
enum class RenderLayer {
    World,
    UI
};

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    //For UI
    RenderLayer renderLayer = RenderLayer::World;
    bool visible = true;
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

struct Gravity {
    float accel{};
    float maxAccel{};
    float currentVelY = 0.0f;
};

struct ScreenWrap {
    bool wrapX{};
    float marginX{};    // Extra pixels before wrapping lower value sooner wrapping
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

//For UI
struct Clickable {
    std::function<void()> onPressed{};
    std::function<void()> onReleased{};
    std::function<void()> onCancelled{};
    bool pressed = false;
};

struct Parent {
    Entity* parent = nullptr;
};

struct Children {
    std::vector<Entity*> children{};
};

enum class LabelType {
    Score,
    PlayerHeight
};

struct Label {
    std::string text{};
    //font maybe
    TTF_Font* font = nullptr;
    SDL_Color color{};
    LabelType type = LabelType::Score;
    std::string textureCacheKey{};
    SDL_Texture* texture = nullptr;
    SDL_FRect dst{};
    bool visible = true;
    bool dirty = false;
};

struct ScoreTracker {
    float maxHeight = 0.0f;
    int score = 0;
};

struct PlayerTag {};

struct ProjectileTag {};

#endif //PROJECT_COMPONENT_H
