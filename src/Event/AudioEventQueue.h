//
// Created by jerry on 2025-11-29.
//

#ifndef GAMEENGINE_AUDIOEVENTQUEUE_H
#define GAMEENGINE_AUDIOEVENTQUEUE_H

#include <memory>
#include <queue>
#include <string>
#include "manager/AudioManager.h"

class AudioEvent {
    std::string name;

    public:
    explicit AudioEvent(const std::string &name) : name(name){}

    void execute() const {
        AudioManager::playSfx(name);
    }
};

class AudioEventQueue {
    std::queue<std::unique_ptr<AudioEvent>> events;
public:
    void pushed(std::unique_ptr<AudioEvent> e) {
        events.push(std::move(e));
    }

    void process() {
        while (!events.empty()) {
            events.front()->execute();
            events.pop();
        }
    }
};

#endif //GAMEENGINE_AUDIOEVENTQUEUE_H