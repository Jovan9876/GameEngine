//
// Created by Jovan on 2025-11-02.
//

#ifndef GAMEENGINE_SCREENWRAPSYSTEM_H
#define GAMEENGINE_SCREENWRAPSYSTEM_H

#include <vector>
#include <memory>
#include "Entity.h"
#include "Component.h"

class ScreenWrapSystem {
public:
    ScreenWrapSystem() = default;

    void setWindowSize(int w, int h) {
        windowWidth = w;
        windowHeight = h;
    }

    void update(std::vector<std::unique_ptr<Entity>>& entities) {
        if (windowWidth <= 0 || windowHeight <= 0) return;

        for (auto& e : entities) {
            if (e->hasComponent<ScreenWrap>() && e->hasComponent<Transform>()) {
                auto& t = e->getComponent<Transform>();
                auto& sw = e->getComponent<ScreenWrap>();

                // Get sprite width
                float spriteWidth = 0.0f;
                if (e->hasComponent<Sprite>()) {
                    spriteWidth = e->getComponent<Sprite>().dst.w;
                }

                // Use center of player to determine wrap
                float halfW = spriteWidth * 0.5f;
                float centerX = t.position.x + halfW;

                // LEFT -> RIGHT
                // If the sprite's center goes past the left edge, place it on right
                if (centerX < -sw.marginX) {
                    t.position.x = static_cast<float>(windowWidth) - halfW;
                }

                // RIGHT -> LEFT
                // If the sprite's center goes past the right edge, place it on left
                else if (centerX > windowWidth + sw.marginX) {
                    t.position.x = -halfW;
                }
            }
        }
    }

private:
    int windowWidth{0};
    int windowHeight{0};
};

#endif //GAMEENGINE_SCREENWRAPSYSTEM_H