//
// Created by jovan on 2025-09-30.
//

#include "CollisionSystem.h"

#include <iostream>

#include "Collision.h"
#include "World.h"

// 1. Positioning the collider with the transform
// 2. Checking for collisions

void CollisionSystem::update(World &world) {
    // Get a list of entities that have colliders and transforms
    const std::vector<Entity *> collidables = queryCollidables(world.getEntities());

    // Update all collider positions first
    for (auto entitiy: collidables) {
        auto& t = entitiy->getComponent<Transform>();
        auto& c = entitiy->getComponent<Collider>();
        c.rect.x = t.position.x;
        c.rect.y = t.position.y;
    }


    // Outer-loop
    for (size_t i = 0; i < collidables.size(); i++) {

        auto entityA = collidables[i];
        auto &colliderA = entityA->getComponent<Collider>();

        // Check for collider collision
        // Inner-loop
        for (size_t j = i + 1; j < collidables.size(); j++) {
            auto &entityB = collidables[j];
            auto &colliderB = entityB->getComponent<Collider>();

            if (Collision::AABB(colliderA, colliderB)) {
                // std::cout << colliderA.tag << "HIT:" << colliderB.tag << std::endl;
                world.getEventManager().emit(CollisionEvent{entityA, entityB});
            }
        }
    }
}

std::vector<Entity *> CollisionSystem::queryCollidables(const std::vector<std::unique_ptr<Entity> > &entities) {
    std::vector<Entity *> collidables;
    for (auto &e: entities) {
        if (e->hasComponent<Transform>() && e->hasComponent<Collider>()) {
            collidables.push_back(e.get());
        }
    }

    return collidables;
}
