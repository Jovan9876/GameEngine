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

    bool isPaused = false;
    Entity* pauseOverlay;

    void update(const float dt, const SDL_Event &e) {
        //Testing pause function
        // if (isPaused) {
        //     togglePause(isPaused);
        // }
        world.update(dt, e, isPaused);
    }

    void render() {
        world.render();
    }

    World world;

    const std::string &getName() const { return name; }
    void togglePause();

private:
    std::string name;

    Entity& createScoreLabel();

    void createProjectile(Vector2D pos, Vector2D dir, int speed);

    // void initMainMenu(int windowWidth, int windowHeight);
    // int initGameplay(const char* mappath, int windowWidth, int windowHeight);

    Entity& createSettingsOverlay(int windowWidth, int windowHeight);
    // Entity& createCogButton(int windowWidth, int windowHeight, Entity& overlay);
    void createSettingsUIComponents(Entity& overlay);
    void toggleSettingsOverlayVisibility(Entity& overlay);
};

#endif //PROJECT_SCENE_H
