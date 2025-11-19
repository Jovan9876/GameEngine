//
// Created by Connie Wu on 2025-11-17.
//

#ifndef GAMEENGINE_PLATFORMSYSTEM_H
#define GAMEENGINE_PLATFORMSYSTEM_H

#include <memory>
#include <vector>
#include "Entity.h"

class PlatformSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt);
};

#endif //GAMEENGINE_PLATFORMSYSTEM_H