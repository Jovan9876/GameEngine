//
// Created by Trevo on 2025-11-17.
//

#include "MapGeneration.h"
#include <ctime>

MapGeneration::MapGeneration(unsigned int seed) {
    if (seed == 0) {
        seed = static_cast<unsigned int>(time(nullptr));
    }
    rng.seed(seed);
}

void MapGeneration::setSeed(unsigned int seed) {
    rng.seed(seed);
}

int MapGeneration::randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

float MapGeneration::randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

void MapGeneration::generatePlatforms(Map& map, int mapWidth, int mapHeight, int numPlatforms) {
    map.width = mapWidth;
    map.height = mapHeight;
    map.tileData = std::vector(mapHeight, std::vector<int>(mapWidth, 0));
    map.colliders.clear();

    float currentY = static_cast<float>(mapHeight * 32 - 64);

    for (int i = 0; i < numPlatforms; i++) {
        int platformWidth = randomInt(minWidth, maxWidth);

        int maxX = mapWidth - platformWidth;
        int tileX = randomInt(0, maxX);
        int tileY = static_cast<int>(currentY / 32);

        for (int j = 0; j < platformWidth; j++) {
            if (tileX + j >= 0 && tileX + j < mapWidth && tileY >= 0 && tileY < mapHeight) {
                map.tileData[tileY][tileX + j] = 2;
            }
        }

        Collider c;
        c.rect.x = static_cast<float>(tileX * 32);
        c.rect.y = static_cast<float>(tileY * 32);
        c.rect.w = static_cast<float>(platformWidth * 32);
        c.rect.h = 32.0f;
        map.colliders.push_back(c);

        currentY -= randomFloat(minGapY, maxGapY);
    }
}