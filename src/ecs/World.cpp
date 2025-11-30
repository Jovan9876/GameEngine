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

        player = nullptr;
        Entity* platform = nullptr;

        if (colliderA.tag == "player" && colliderB.tag == "platform") {
            player   = collision.entityA;
            platform = collision.entityB;
        } else if (colliderA.tag == "platform" && colliderB.tag == "player") {
            platform = collision.entityA;
            player   = collision.entityB;
        }

        //player vs platform
        if (player && platform) {
            // Need gravity + colliders + platform component
            if (!(player->hasComponent<Gravity>() && player->hasComponent<Collider>() &&
                  platform->hasComponent<Collider>() && platform->hasComponent<Platform>())) {
                return;
            }

            auto& playerGravity = player->getComponent<Gravity>();
            auto& playerCollider = player->getComponent<Collider>();
            auto& platformCollider = platform->getComponent<Collider>();
            auto& platformComponent = platform->getComponent<Platform>();

            float playerBottom = playerCollider.rect.y + playerCollider.rect.h;
            float platformTop  = platformCollider.rect.y;
            const float tolerance = 5.0f;

            bool fromAbove = (playerBottom <= platformTop + tolerance);

            // SDL: y+ is down, so falling means currentVelY > 0
            if (fromAbove && playerGravity.currentVelY > 0.0f) {
                //upward bounce using maxaccel
                playerGravity.currentVelY = -playerGravity.maxAccel;

                if (platformComponent.type == Platform::Type::Breakable &&
                    platform->hasComponent<BreakablePlatform>()) {

                    auto& br = platform->getComponent<BreakablePlatform>();
                    if (!br.triggered) {
                        br.triggered = true;
                        br.timer = 0.0f;
                    }
                }
            }

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
