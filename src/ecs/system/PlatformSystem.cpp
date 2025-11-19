//
// Created by Connie Wu on 2025-11-18.
//

#include "PlatformSystem.h"
#include "Component.h"
#include <cmath>

namespace {
    constexpr float EPS = 0.001f;
}

void PlatformSystem::update(std::vector<std::unique_ptr<Entity>>& entities, float dt)
{
    for (auto &entity : entities) {
        Entity* e = entity.get();
        if (!e->isActive()) continue;

        //moving platforms
        if (e->hasComponent<Transform>() &&
            e->hasComponent<Platform>()  &&
            e->hasComponent<MovingPlatform>()) {

            auto& t      = e->getComponent<Transform>();
            auto& plat   = e->getComponent<Platform>();
            auto& moving = e->getComponent<MovingPlatform>();

            if (plat.type == Platform::Type::Moving) {
                Vector2D target = moving.onroute ? moving.endPoint : moving.startPoint;
                Vector2D diff   = target - t.position;
                float    dist   = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                if (dist > EPS) {
                    Vector2D dir  = diff / dist;
                    float    step = moving.speed * dt;

                    if (step >= dist) {
                        t.position     = target;
                        moving.onroute = !moving.onroute;
                    } else {
                        t.position += dir * step;
                    }
                } else {
                    moving.onroute = !moving.onroute;
                }
            }
            }

        //breakable platforms
        if (e->hasComponent<Platform>() &&
            e->hasComponent<BreakablePlatform>()) {

            auto& plat = e->getComponent<Platform>();
            auto& br   = e->getComponent<BreakablePlatform>();

            if (plat.type != Platform::Type::Breakable) continue;
            if (!br.triggered) continue;

            br.timer += dt;

            if (!br.broken && br.timer >= br.breakDelay) {
                br.broken = true;
                if (e->hasComponent<Collider>()) {
                    auto& col = e->getComponent<Collider>();
                    col.rect.w = 0.0f;
                    col.rect.h = 0.0f;
                }
            }

            if (br.timer >= br.breakDelay + br.destroyDelay) {
                e->destroy();
            }
        }
    }
}
