//
// Created by jerry on 2025-11-17.
//

#ifndef GAMEENGINE_HUDSYSTEM_H
#define GAMEENGINE_HUDSYSTEM_H

#include <sstream>

#include "Component.h"
#include "Entity.h"

class HUDSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        Entity* playerEntity = nullptr;

        //find player
        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>()) {
                playerEntity = e.get();
                break;
            }
        }

        if (!playerEntity) return;
        auto& playerTransform = playerEntity->getComponent<Transform>();

        for (auto& e : entities) {
            if (e->hasComponent<Label>()) {
                auto& label = e.get()->getComponent<Label>();

                //update player position
                if (label.type == LabelType::Score) {
                    std::stringstream ss;
                    ss << "Player position aka Score" << playerTransform.position.y;
                    label.text = ss.str();
                    label.dirty = true;
                }
            }
        }


    }

};

#endif //GAMEENGINE_HUDSYSTEM_H