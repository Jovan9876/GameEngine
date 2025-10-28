//
// Created by jovan on 2025-09-23.
//

#include "Vector2D.h"

#include <cmath>
#include <math.h>


// Member operator function so we can use the vector2D that want to change on the left  hand side
Vector2D &Vector2D::operator+=(const Vector2D &vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

// Subtract a vector from this one -=
Vector2D &Vector2D::operator-=(const Vector2D &vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

// Member operator function so we can use the Vector2D on the left hand side
Vector2D Vector2D::operator*(float scaler) const {
    return Vector2D(x * scaler, y * scaler);
}

// Non-member operator function to make use of putting the Vector2D on the right
// Hand side
Vector2D operator*(float scaler, Vector2D &vec) {
    return Vector2D(vec.x * scaler, vec.y * scaler);
}

// Scale this vector *=
Vector2D &Vector2D::operator*=(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

// Scale this vector /=
Vector2D &Vector2D::operator/=(float scalar) {
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

// Vector + Vector
Vector2D Vector2D::operator+(const Vector2D &vec) const {
    return Vector2D(this->x + vec.x, this->y + vec.y);
}

// Vector - Vector
Vector2D Vector2D::operator-(const Vector2D &vec) const {
    return Vector2D(this->x - vec.x, this->y - vec.y);
}

// Vector / scalar
Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(this->x / scalar, this->y / scalar);
}

// Equality
bool Vector2D::operator==(const Vector2D &vec) const {
    return this->x == vec.x && this->y == vec.y;
}

// Inequality
bool Vector2D::operator!=(const Vector2D &vec) const {
    return !(*this == vec);
}

// Negation
Vector2D Vector2D::operator-() const {
    return Vector2D(-this->x, -this->y);
}


Vector2D &Vector2D::normalize() {
    float length = std::sqrt(x * x + y * y);

    if (length > 0) {
        this->x /= length;
        this->y /= length;
    }

    return *this;
}
