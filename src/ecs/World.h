//
// Created by jovan on 2025-09-16.
//

#ifndef PROJECT_WORLD_H
#define PROJECT_WORLD_H
#include <memory>
#include <vector>

#include "AnimationSystem.h"
#include "CollisionSystem.h"
#include "Entity.h"
#include "EventManager.h"
#include "KeyboardInputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "Map.h"
#include "CameraSystem.h"
#include "DestructionSystem.h"
#include "SpawnTimerSystem.h"
#include "GravitySystem.h"
#include "HUDSystem.h"
#include "PreRenderSystem.h"
#include "ScreenWrapSystem.h"
#include "UIRenderSystem.h"
#include "Event/AudioEventQueue.h"

class World {
    Map map;
    std::vector<std::unique_ptr<Entity> > entities;
    std::vector<std::unique_ptr<Entity> > defferedEntities;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    CollisionSystem collisionSystem;
    AnimationSystem animationSystem;
    CameraSystem cameraSystem;
    EventManager eventManager;
    SpawnTimerSystem spawnTimerSystem;
    DestructionSystem destructionSystem;
    GravitySystem gravitySystem;
    ScreenWrapSystem screenWrapSystem;
    int windowWidth{0};
    int windowHeight{0};

    //HUD
    UIRenderSystem uiRenderSystem;
    HUDSystem hudSystem;
    PreRenderSystem preRenderSystem;

    //Audio
    AudioEventQueue audioEventQueue;

public:
    World();

    void update(float dt, const SDL_Event &event, bool &isPaused) {
        keyboardInputSystem.update(entities, event, isPaused);

        //Testing pause
        if (!isPaused) {
            movementSystem.update(entities, dt);
            gravitySystem.update(entities, dt);
            screenWrapSystem.update(entities);
            collisionSystem.update(*this);
            animationSystem.update(entities, dt);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, dt);
            destructionSystem.update(entities);
            hudSystem.update(entities);
        }

        audioEventQueue.process();
        //put this last
        preRenderSystem.update(entities);

        synchronizeEntities();
        cleanup();
    }

    void setWindowSize(int w, int h) {
        windowWidth = w;
        windowHeight = h;
        screenWrapSystem.setWindowSize(w, h);
    }

    void render() {
        for (auto &entity: entities) {
            if (entity->hasComponent<Camera>()) {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }
        renderSystem.render(entities);
        uiRenderSystem.render(entities);
    }

    Entity &createEntity() {
        // We use emplace instead of push so we don't create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    std::vector<std::unique_ptr<Entity> > &getEntities() {
        return entities;
    }

    Entity &createDeferredEntity() {
        defferedEntities.emplace_back(std::make_unique<Entity>());
        return *defferedEntities.back();
    }

    void cleanup() {
        // Use a lambda predicate to remove all inactive entities
        std::erase_if(
            entities,
            [](std::unique_ptr<Entity> &e) {
                return !e->isActive();
            });
    }

    void synchronizeEntities() {
        if (!defferedEntities.empty()) {
            // Push back all deferred entities to the entities vector
            // Using move so we dontt create a copy
            std::move(
                defferedEntities.begin(),
                defferedEntities.end(),
                std::back_inserter(entities)
            );
            // Clearing the creation buffer
            defferedEntities.clear();
        };
    }

    //For Audio
    AudioEventQueue& getAudioEventQueue(){return audioEventQueue;};

    EventManager &getEventManager() { return eventManager; }
    Map &getMap() { return map; }
};

#endif //PROJECT_WORLD_H
