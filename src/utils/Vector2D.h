//
// Created by jovan on 2025-09-23.
//

#ifndef PROJECT_VECTOR2D_H
#define PROJECT_VECTOR2D_H


class Vector2D {
public:
    float x = 0.0f;
    float y = 0.0f;

    Vector2D() {
        x = 0.0f;
        y = 0.0f;
    }

    Vector2D(float x, float y) : x(x), y(y) {}

    // Member operator function (*)
    // Vector2D has to be on the left, float on the right
    Vector2D operator*(float scaler) const;

    // Vector2D is on the right, and float is on the left
    // Friend makes it a non-member function
    friend Vector2D operator*(float scaler, Vector2D &vec);

    // Member operator function (+=)
    // Vector that we want to return on the left
    // Another vector on the right
    Vector2D &operator+=(const Vector2D &vec);

    Vector2D &operator-=(const Vector2D &vec);

    Vector2D &operator*=(float scalar);
    Vector2D &operator/=(float scalar);

    Vector2D operator+(const Vector2D &vec) const;
    Vector2D operator-(const Vector2D &vec) const;

    Vector2D operator/(float scalar) const;

    friend Vector2D operator*(float scalar, const Vector2D &vec);

    bool operator==(const Vector2D &vec) const;
    bool operator!=(const Vector2D &vec) const;

    Vector2D operator-() const;

    Vector2D &normalize();
};


#endif //PROJECT_VECTOR2D_H
