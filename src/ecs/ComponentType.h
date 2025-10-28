//
// Created by jovan on 2025-09-16.
//

#ifndef PROJECT_COMPONENTTYPE_H
#define PROJECT_COMPONENTTYPE_H
#include <cstddef>

// Creating a type alias for std:size_t
// size_t is designed to represent sizes and indices because its unsigned
// And its very large (long, long)
using ComponentTypeID = std::size_t;

// When you have a free function and defined in a header, each .cpp file that includes this header
// (ComponentType.h) would get its own definition causing a linker error
// So using inline merges them into one definition
// Global counter generator, returns a unique number everytime we call it
// Making id static, means it will keep its value through multiple calls of this function
inline ComponentTypeID getComponentTypeID() {
    static ComponentTypeID id = 0;
    return id++;
}

// Can overload this function because they have different signatures
// Template allows us to use generic types
// Template is inline by default
// getComponentTypeID<position>() -> would always return 0
// getComponentTypeID<Health>() -> would always return 1
// A static local variable is created and initialized once
template <typename T>
ComponentTypeID getComponentTypeID() {
    static ComponentTypeID typeID = getComponentTypeID();
    return typeID;
}

#endif //PROJECT_COMPONENTTYPE_H