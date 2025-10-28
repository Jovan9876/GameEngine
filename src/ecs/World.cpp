//
// Created by jovan on 2025-09-30.
//

#include "World.h"

#include "Game.h"
#include "utils/CollisionObservers.h"

World::World() {
    // Subscribe to the collision events
    eventManager.subscribe([this](const CollisionEvent &collision) {
        Entity *sceneStateEntity = nullptr;

        // Find scene state
        for (auto &e: entities) {
            if (e->hasComponent<SceneState>()) {
                sceneStateEntity = e.get();
                break;
            }
        }

        if (!sceneStateEntity) return;

        if (collision.entityA == nullptr || collision.entityB == nullptr) {
            return;
        }

        if (!(collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())) {
            return;
        }

        auto &colliderA = collision.entityA->getComponent<Collider>();
        auto &colliderB = collision.entityB->getComponent<Collider>();

        Entity *player = nullptr;
        Entity *item = nullptr;
        Entity *wall = nullptr;
        Entity *projectile = nullptr;

        if (colliderA.tag == "player" && colliderB.tag == "item") {
            player = collision.entityA;
            item = collision.entityB;
        } else if (colliderA.tag == "item" && colliderB.tag == "player") {
            item = collision.entityA;
            player = collision.entityB;
        }

        if (player && item) {
            item->destroy();
            // Scene state
            auto &sceneState = sceneStateEntity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }

        // Player vs wall
        if (colliderA.tag == "player" && colliderB.tag == "wall") {
            player = collision.entityA;
            wall = collision.entityB;
        } else if (colliderA.tag == "wall" && colliderB.tag == "player") {
            wall = collision.entityA;
            player = collision.entityB;
        }

        if (player && wall) {
            // Stop the player
            auto &t = player->getComponent<Transform>();
            t.position = t.oldPosition;
        }

        // Player vs projectile
        if (colliderA.tag == "player" && colliderB.tag == "projectile") {
            player = collision.entityA;
            projectile = collision.entityB;
        } else if (colliderA.tag == "projectile" && colliderB.tag == "player") {
            projectile = collision.entityA;
            player = collision.entityB;
        }

        if (player && projectile) {
            player->destroy();
            // Change scenes defer
            Game::onSceneChangeRequest("gameover");
        }
    });

    eventManager.subscribe(PrintCollisionObserver);
}
