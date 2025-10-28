//
// Created by jovan on 2025-09-30.
//

#ifndef PROJECT_COLLISIONSYSTEM_H
#define PROJECT_COLLISIONSYSTEM_H
#include <memory>
#include <vector>
#include "Entity.h"

// Forward declaring
class World;

class CollisionSystem {
public:
    void update(World &world);

private:
    std::vector<Entity *> queryCollidables(const std::vector<std::unique_ptr<Entity> > &entities);
};

#endif //PROJECT_COLLISIONSYSTEM_H
