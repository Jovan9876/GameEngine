//
// Created by jovan on 2025-10-21.
//

#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H
#include <SDL3/SDL_events.h>
#include "World.h"


class Scene {
public:
    Scene(const char *sceneName, const char *mapPath, int windowWidth, int windowHeight);

    void update(const float dt, const SDL_Event &e) {
        world.update(dt, e);
    }

    void render() {
        world.render();
    }

    World world;

    const std::string &getName() const { return name; }

private:
    std::string name;

    Entity& createScoreLabel();

    void createProjectile(Vector2D pos, Vector2D dir, int speed);
};

#endif //PROJECT_SCENE_H
