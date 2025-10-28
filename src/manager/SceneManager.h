//
// Created by jovan on 2025-10-21.
//

#ifndef PROJECT_SCENEMANAGER_H
#define PROJECT_SCENEMANAGER_H
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

#include "Scene.h"


struct SceneParams {
    const char *name;
    const char *mapPath;
    int windowWidth;
    int windowHeight;
};


class SceneManager {
    std::unordered_map<std::string, SceneParams> sceneParams;
    std::string pendingScene;

    void changeScene(const std::string &name) {
        auto it = sceneParams.find(name);
        if (it != sceneParams.end()) {
            const auto &params = it->second;
            // Create scene object
            // Build scene
            currentScene = std::make_unique<Scene>(
                params.name,
                params.mapPath,
                params.windowWidth,
                params.windowHeight
            );
        } else {
            std::cerr << "Scene " << name << " not found!" << std::endl;
        }
    }

public:
    std::unique_ptr<Scene> currentScene;

    void loadScene(const char *sceneName, const char *mapPath, int windowWidth, int windowHeight) {
        sceneParams[sceneName] = {sceneName, mapPath, windowWidth, windowHeight};
    }

    void changeSceneDeferred(const std::string &name) {
        pendingScene = name;
    }

    void update(const float dt, const SDL_Event &e) {
        if (currentScene) currentScene->update(dt, e); // All world updates
        if (!pendingScene.empty()) {
            changeScene(pendingScene);
            pendingScene.clear();
        }

    }

    void render () const {
        if (currentScene) currentScene->render();
    }

};


#endif //PROJECT_SCENEMANAGER_H
