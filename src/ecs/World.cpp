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

        // player vs platform
        if (player && platform) {
            // Need gravity + transform + collider + platform components
          if (!(player->hasComponent<Gravity>()   &&
                player->hasComponent<Transform>() &&
                player->hasComponent<Collider>()  &&
                platform->hasComponent<Collider>() &&
                platform->hasComponent<Platform>())) {
              return;
          }

          auto &playerGravity    = player->getComponent<Gravity>();
          auto &playerTransform  = player->getComponent<Transform>();
          auto &playerCollider   = player->getComponent<Collider>();
          auto &platformCollider = platform->getComponent<Collider>();
          auto &platformComponent= platform->getComponent<Platform>();

          float prevBottom  = playerTransform.oldPosition.y + playerCollider.rect.h;
          float newBottom   = playerTransform.position.y    + playerCollider.rect.h;
          float platformTop = platformCollider.rect.y;

         // Check that we're above plat
          bool crossedTop = (prevBottom <= platformTop) && (newBottom >= platformTop);

          // SDL: y+ is down, so falling means currentVelY > 0
          if (crossedTop && playerGravity.currentVelY > 0.0f) {

              // Snap player onto platform, to correct fall through
              playerTransform.position.y = platformTop - playerCollider.rect.h;
              playerCollider.rect.y      = playerTransform.position.y;

             // Bounce
              playerGravity.currentVelY = -playerGravity.maxAccel;

             // Breakable usable once
              if (platformComponent.type == Platform::Type::Breakable &&
                  platform->hasComponent<BreakablePlatform>()) {

                  auto &br = platform->getComponent<BreakablePlatform>();
                  if (!br.triggered) {
                      br.triggered = true;
                      br.timer    = 0.0f;
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

void World::checkFallOffScreen(const std::vector<std::unique_ptr<Entity>>& entities) {
    Entity *playerEntity = nullptr;

    for (auto &e: entities) {
        if (e->hasComponent<PlayerTag>()) {
            playerEntity = e.get();
            break;
        }
    }

    for (auto &e: entities) {
        if (e->hasComponent<Camera>()) {
            auto &cam = e->getComponent<Camera>();
            //Check game over
            float cameraBottom = cam.view.y + cam.view.h;

            if (playerEntity->getComponent<Transform>().position.y > cameraBottom) {
                std::cout << "Game Over\n";
                Game::onSceneChangeRequest("gameover");
            }
        }
    }


}