//
// Created by jovan on 2025-10-14.
//

#ifndef PROJECT_ASSETMANAGER_H
#define PROJECT_ASSETMANAGER_H
#include <string>
#include <unordered_map>
#include "Component.h"

class AssetManager {
    static std::unordered_map<std::string, Animation> animations;

    static Animation loadAnimationFromXML(const char *path);

public:
    static void loadAnimation(const std::string &clipName, const char *path);

    static const Animation &getAnimation(const std::string &clipName);
};

#endif //PROJECT_ASSETMANAGER_H
