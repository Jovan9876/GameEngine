//
// Created by jovan on 2025-09-16.
//

#ifndef PROJECT_RENDERSYSTEM_H
#define PROJECT_RENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "TextureManager.h"

class RenderSystem {
public:
    void render(const std::vector<std::unique_ptr<Entity> > &entities) {

        Entity* cameraEntity = nullptr;
        // Find camera
        for (auto& e : entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return; // No camera
        auto& cam = cameraEntity->getComponent<Camera>();

        for (auto &entity: entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto &t = entity->getComponent<Transform>();
                auto &sprite = entity->getComponent<Sprite>();

                // Converting from world space to screen space
                sprite.dst.x = t.position.x - cam.view.x;
                sprite.dst.y = t.position.y - cam.view.y;


                // If the entity has an animation component, update the src rect
                if (entity->hasComponent<Animation>()) {
                    auto& anim = entity->getComponent<Animation>();
                    sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                }


                TextureManager::draw(sprite.texture, sprite.src, sprite.dst);
            }
        }
    }
};

#endif //PROJECT_RENDERSYSTEM_H
