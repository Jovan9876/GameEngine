//
// Created by Jovan on 2025-11-02.
//

#ifndef GAMEENGINE_GRAVITYSYSTEM_H
#define GAMEENGINE_GRAVITYSYSTEM_H

#include <memory>
#include <vector>

#include "Entity.h"
#include "Component.h"

class GravitySystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Gravity>()) {
                auto& t = e->getComponent<Transform>();
                auto& g = e->getComponent<Gravity>();

                // accelerate downward
                g.currentVelY += g.accel * dt;

                // clamp to max fall speed
                if (g.currentVelY > g.maxAccel) {
                    g.currentVelY = g.maxAccel;
                }

                // move the entity down
                t.position.y += g.currentVelY * dt;
            }
        }
    }
};

#endif //GAMEENGINE_GRAVITYSYSTEM_H