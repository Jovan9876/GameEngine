//
// Created by jovan on 2025-09-30.
//

#include <iostream>
#include "CollisionObservers.h"

#include "Component.h"

void PrintCollisionObserver(const CollisionEvent &collision) {
    if (collision.entityA && collision.entityB) {
        // Check if both have Collider component
        if (collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>()) {
            auto &colA = collision.entityA->getComponent<Collider>();
            auto &colB = collision.entityB->getComponent<Collider>();

            // std::cout << "A collision occurred between "
            //         << colA.tag << " and " << colB.tag
            //         << std::endl;
        }
    }
}
