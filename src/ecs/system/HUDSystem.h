//
// Created by jerry on 2025-11-17.
//

#ifndef GAMEENGINE_HUDSYSTEM_H
#define GAMEENGINE_HUDSYSTEM_H

#include <sstream>

#include "Component.h"
#include "Entity.h"

class HUDSystem {
    bool firstUpdate = true;
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
                auto& scoreTracker = e.get()->getComponent<ScoreTracker>();

                //update player position
                if (label.type == LabelType::Score) {
                    std::stringstream ss;

                    if (firstUpdate) {
                        scoreTracker.maxHeight = playerTransform.position.y;
                        firstUpdate = false;
                    }

                    //Put score calculation here
                    if (playerTransform.position.y < scoreTracker.maxHeight) {
                        float heightDifference = scoreTracker.maxHeight - playerTransform.position.y;
                        scoreTracker.maxHeight = playerTransform.position.y;
                        scoreTracker.score += static_cast<int>(heightDifference / 5);
                    };

                    ss << "Score: " << scoreTracker.score;
                    label.text = ss.str();
                    label.dirty = true;
                }
            }
        }


    }

};

#endif //GAMEENGINE_HUDSYSTEM_H