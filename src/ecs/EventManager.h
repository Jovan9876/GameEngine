//
// Created by jovan on 2025-09-30.
//

#ifndef PROJECT_EVENTMANAGER_H
#define PROJECT_EVENTMANAGER_H
#include "Entity.h"
#include <functional>

struct CollisionEvent {
    Entity *entityA = nullptr;
    Entity *entityB = nullptr;
};


// Observer Pattern
class EventManager {
public:
    void emit(const CollisionEvent &event) const {
        for (const auto &listener: collisionListeners) {
            listener(event);
        }
    }

    // template<typename EventType>
    // void emit(const EventType& event) {
    //     // Retrieve the list of subscribers to certain events
    //     auto &listeners = getListeners<EventType>();
    //
    //     // Loop all the subsribers to certain events
    //     for (auto &listener: listeners) {
    //         listener(event); // Invoke the function
    //     }
    // }

    // // template<typename EventType>
    // // void subscribe(std::function<void(const EventType &)> callback) {
    // //     // Pass in the callback, wrapper/subscription to the list of subscriptions
    // //     getListeners<EventType>().push_back(callback);
    // }

    void subscribe(std::function<void(const CollisionEvent &)> callback) {
        collisionListeners.emplace_back(callback);
    }

private:
    // Each event type has its own std:vector of listeners without explicit management
    // Done using static local
    // std:function<void(const EventType&)> is the callable wrapper
    // Can hold anything that can be called like a function (lambda, free function)
    // template<typename EventType>
    // std::vector<std::function<void(const EventType&)>> &getListeners() {
    //     static std::vector<std::function<void(const EventType &)> > listeners;
    //     return listeners;
    // }

    std::vector<std::function<void(const CollisionEvent &)> > collisionListeners;
};

#endif //PROJECT_EVENTMANAGER_H
