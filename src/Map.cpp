//
// Created by jovan on 2025-09-09.
//

#include "Map.h"
#include "TextureManager.h"
#include <sstream>
#include <tinyxml2.h>
#include <cmath>

void Map::load(const char *path, SDL_Texture *ts) {
    tileset = ts;
    tileData.clear();
    colliders.clear();
    itemSpawnPoints.clear();

    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    // Parse width and height of map
    auto *mapNode = doc.FirstChildElement("map");
    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");

    // Parse terrain data
    auto *layer = mapNode->FirstChildElement("layer");
    auto *data = layer->FirstChildElement("data");
    std::string csv = data->GetText();

    std::stringstream ss(csv);
    tileData = std::vector(height, std::vector<int>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::string val;
            // Read characters from a string stream into val
            // Until it hits a comma or until end of stream
            if (!std::getline(ss, val, ',')) break;
            tileData[i][j] = std::stoi(val); // Stoi is a string to integer conversion
        }
    }

    for (auto *og = mapNode->FirstChildElement("objectgroup");
         og != nullptr;
         og = og->NextSiblingElement("objectgroup")) {
        const char *name = og->Attribute("name");

        bool isCollision = std::strcmp(name, "Collision Layer") == 0;

        bool isItems = std::strcmp(name, "Item Layer") == 0;

        if (isCollision) {
            for (auto *obj = og->FirstChildElement("object");
                 obj;
                 obj = obj->NextSiblingElement("object")) {

                Collider c;
                c.rect.x = obj->FloatAttribute("x");
                c.rect.y = obj->FloatAttribute("y");
                c.rect.w = obj->FloatAttribute("width");
                c.rect.h = obj->FloatAttribute("height");
                colliders.push_back(c);
            }
        } else if (isItems) {
            for (auto *obj = og->FirstChildElement("object");
                 obj;
                 obj = obj->NextSiblingElement("object")) {

                SDL_FPoint p;
                p.x = obj->FloatAttribute("x");
                p.y = obj->FloatAttribute("y");
                itemSpawnPoints.push_back(p);
            }
        }
    }
}

void Map::draw(const Camera &cam) {
    SDL_FRect src{}, dest{};

    dest.w = dest.h = 32;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int type = tileData[row][col];

            float worldX = static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            // Move the tiles or map relative to the camera
            // Convert from world space to screen space
            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            switch (type) {
                case 1:
                    // Dirt
                    src.x = 0;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 2:
                    // Grass
                    src.x = 32;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 4:
                    // Water
                    src.x = 32;
                    src.y = 32;
                    src.w = 32;
                    src.h = 32;
                    break;
                default:
                    break;
            }
            TextureManager::draw(tileset, &src, &dest);
        }
    }
}
