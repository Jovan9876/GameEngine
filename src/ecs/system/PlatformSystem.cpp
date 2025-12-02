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

                auto &t      = e->getComponent<Transform>();
                auto &plat   = e->getComponent<Platform>();
                auto &moving = e->getComponent<MovingPlatform>();

                if (plat.type == Platform::Type::Moving) {
                    Vector2D target = moving.moveToB ? moving.endPoint : moving.startPoint;
                    Vector2D diff   = target - t.position;
                    float    dist   = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                    if (dist > 0.001f) {
                        Vector2D dir  = diff / dist;
                        float    step = moving.speed * dt;

                        if (step >= dist) {
                            t.position   = target;
                            moving.moveToB = !moving.moveToB; // flip direction
                        } else {
                            t.position += dir * step;
                        }
                    } else {
                        moving.moveToB = !moving.moveToB;
                    }

                    // move sprite and collider with the transform vals
                    if (e->hasComponent<Sprite>()) {
                        auto &s = e->getComponent<Sprite>();
                        s.dst.x = t.position.x;
                        s.dst.y = t.position.y;
                    }
                    if (e->hasComponent<Collider>()) {
                        auto &col = e->getComponent<Collider>();
                        col.rect.x = t.position.x;
                        col.rect.y = t.position.y;
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
