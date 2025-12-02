//
// Created by Trevo on 2025-11-17.
//

#ifndef GAMEENGINE_MAPGENERATION_H
#define GAMEENGINE_MAPGENERATION_H

#include "Map.h"
#include <random>

class MapGeneration {
public:
    MapGeneration(unsigned int seed = 0);

    void generatePlatforms(Map& map, int mapWidth, int mapHeight, int numPlatforms);

    void setSeed(unsigned int seed);

    float minGapY = 60.0f;
    float maxGapY = 120.0f;
    int minWidth = 2;
    int maxWidth = 5;

private:
    std::mt19937 rng;

    int randomInt(int min, int max);
    float randomFloat(float min, float max);
};
#endif //GAMEENGINE_MAPGENERATION_H