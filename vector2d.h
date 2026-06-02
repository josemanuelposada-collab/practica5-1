#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QtGlobal>
#include <cmath>

class Vector2D
{
public:
    double x;
    double y;

    Vector2D(double xValue = 0.0, double yValue = 0.0)
        : x(xValue), y(yValue)
    {
    }

    double length() const
    {
        return std::sqrt(x * x + y * y);
    }

    double lengthSquared() const
    {
        return x * x + y * y;
    }

    Vector2D normalized() const
    {
        const double len = length();
        if (qFuzzyIsNull(len)) {
            return Vector2D();
        }
        return Vector2D(x / len, y / len);
    }

    double dot(const Vector2D &other) const
    {
        return x * other.x + y * other.y;
    }

    Vector2D operator+(const Vector2D &other) const
    {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D &other) const
    {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(double scalar) const
    {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(double scalar) const
    {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D &operator+=(const Vector2D &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
};

#endif
