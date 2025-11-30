//
// Created by jerry on 2025-11-17.
//

#ifndef GAMEENGINE_UIRENDERSYSTEM_H
#define GAMEENGINE_UIRENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "RenderUtils.h"

class UIRenderSystem {
    public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities) {

        for (auto& e : entities) {
            if (e->hasComponent<Transform>()) {

                auto& transform = e->getComponent<Transform>();

                if (e->hasComponent<Sprite>()) {
                    // auto transform = e->getComponent<Transform>();
                    auto sprite = e->getComponent<Sprite>();

                    if (sprite.renderLayer != RenderLayer::UI) continue;

                    sprite.dst.x = transform.position.x;
                    sprite.dst.y = transform.position.y;

                    if (sprite.visible) {
                        SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                        TextureManager::draw(sprite.texture, &sprite.src, &scaledDest);
                    }
                }

                if (e->hasComponent<Label>()) {
                    auto& label = e->getComponent<Label>();

                    label.dst.x = transform.position.x;
                    label.dst.y = transform.position.y;

                    if (label.visible) {
                        SDL_FRect scaledDest = RenderUtils::getScaledDest(label.dst, transform.scale);
                        TextureManager::draw(label.texture, nullptr, &scaledDest);
                    }
                }


                // auto transform = e->getComponent<Transform>();
                // auto sprite = e->getComponent<Sprite>();
                //
                // if (sprite.renderLayer != RenderLayer::UI) continue;
                //
                // //no converting from world space to screen space
            }
        }
    }
};

#endif //GAMEENGINE_UIRENDERSYSTEM_H