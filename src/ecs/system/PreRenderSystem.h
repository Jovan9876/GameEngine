//
// Created by jerry on 2025-11-17.
//

#ifndef GAMEENGINE_PRERENDERSYSTEM_H
#define GAMEENGINE_PRERENDERSYSTEM_H
#include "Component.h"
#include "TextureManager.h"


class PreRenderSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Label>() && entity->getComponent<Label>().dirty) {
                //update label texture
                TextureManager::updateLabel(entity->getComponent<Label>());
            }
        }
    }
};


#endif //GAMEENGINE_PRERENDERSYSTEM_H