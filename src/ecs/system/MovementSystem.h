//
// Created by jovan on 2025-09-16.
//

#ifndef PROJECT_MOVEMENTSYSTEM_H
#define PROJECT_MOVEMENTSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class MovementSystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, float dt) {
        for (auto &entity: entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
                auto &t = entity->getComponent<Transform>();
                auto &v = entity->getComponent<Velocity>();

                // Track previous frames position
                t.oldPosition = t.position;

                Vector2D directionVec = v.direction;

                // Normalize
                directionVec.normalize();

                // Vector2D needs an operator function to multiply a float to itself
                Vector2D velocityVector = directionVec * v.speed;
                t.position += (velocityVector * dt);

            }
        }
    };
};

#endif //PROJECT_MOVEMENTSYSTEM_H
