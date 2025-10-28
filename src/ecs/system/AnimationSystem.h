//
// Created by jovan on 2025-10-14.
//

#ifndef PROJECT_ANIMATIONSYSTEM_H
#define PROJECT_ANIMATIONSYSTEM_H
#include <memory>

#include "Component.h"
#include "Entity.h"

// State system; deciding which clip to use
// Check if the animation has been switched
// Playback system advances the animation

class AnimationSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities, float dt) {
        for (auto &e: entities) {
            if (e->hasComponent<Animation>() && e->hasComponent<Velocity>()) {
                auto &anim = e->getComponent<Animation>();
                auto &velocity = e->getComponent<Velocity>();

                // State system for player
                std::string newClip;
                if (e->hasComponent<PlayerTag>()) {
                    if (velocity.direction.x > 0.0f) {
                        newClip = "walk_right";
                    } else if (velocity.direction.x < 0.0f) {
                        newClip = "walk_left";
                    } else if (velocity.direction.y > 0.0f) {
                        newClip = "walk_down";
                    } else if (velocity.direction.y < 0.0f) {
                        newClip = "walk_up";
                    } else {
                        newClip = "idle";
                    }
                } else if (e->hasComponent<ProjectileTag>()) {
                    newClip = "idle_right";
                }

                // Check if the animation has switched
                // If the chosen clip is different from the current, switch to new clip, reset time and frame index
                if (newClip != anim.currentClip) {
                    anim.currentClip = newClip; // Switch to new clip
                    anim.time = 0.0f; // Reset time to 0
                    anim.currentFrame = 0; // Reset frame index to 0
                }

                // Playback system; advance the animation
                float animFrameSpeed = anim.speed; // How long each animation frame should last
                auto clip = anim.clips[anim.currentClip]; // Retrieve frame data from current clip
                // Advance time
                anim.time += dt; // Every 'game loop frame' we are going to add the accumulated time

                // Once enough time has passed
                if (anim.time >= animFrameSpeed) {
                    anim.time -= animFrameSpeed; // Subtract animFrameSpeed so the extra time isnt lost

                    std::size_t totalAnimationFrames = clip.frameIndices.size();
                    // Advance to the next animation frame (currentAnimationFrame + 1)
                    // Wrap around % so it loops when reaching the end of the clip
                    anim.currentFrame = (anim.currentFrame + 1) % totalAnimationFrames;
                }
            }
        }
    }
};

#endif //PROJECT_ANIMATIONSYSTEM_H
