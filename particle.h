#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector2d.h"
#include <QColor>

class Particle
{
public:
    Particle(int particleId = 0,
             const Vector2D &initialPosition = Vector2D(),
             const Vector2D &initialVelocity = Vector2D(),
             double particleMass = 1.0,
             double particleRadius = 8.0,
             const QColor &particleColor = Qt::white);

    int id() const;
    Vector2D position() const;
    Vector2D velocity() const;
    double mass() const;
    double radius() const;
    QColor color() const;
    bool isActive() const;

    void setPosition(const Vector2D &newPosition);
    void setVelocity(const Vector2D &newVelocity);
    void setMass(double newMass);
    void setRadius(double newRadius);
    void setActive(bool value);

    void move(double deltaTime);
    void absorb(const Particle &other);

private:
    int m_id;
    Vector2D m_position;
    Vector2D m_velocity;
    double m_mass;
    double m_radius;
    QColor m_color;
    bool m_active;
};

#endif
