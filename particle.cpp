#include "particle.h"

#include <algorithm>
#include <cmath>

Particle::Particle(int particleId,
                   const Vector2D &initialPosition,
                   const Vector2D &initialVelocity,
                   double particleMass,
                   double particleRadius,
                   const QColor &particleColor)
    : m_id(particleId),
      m_position(initialPosition),
      m_velocity(initialVelocity),
      m_mass(particleMass),
      m_radius(particleRadius),
      m_color(particleColor),
      m_active(true)
{
}

int Particle::id() const
{
    return m_id;
}

Vector2D Particle::position() const
{
    return m_position;
}

Vector2D Particle::velocity() const
{
    return m_velocity;
}

double Particle::mass() const
{
    return m_mass;
}

double Particle::radius() const
{
    return m_radius;
}

QColor Particle::color() const
{
    return m_color;
}

bool Particle::isActive() const
{
    return m_active;
}

void Particle::setPosition(const Vector2D &newPosition)
{
    m_position = newPosition;
}

void Particle::setVelocity(const Vector2D &newVelocity)
{
    m_velocity = newVelocity;
}

void Particle::setMass(double newMass)
{
    m_mass = newMass;
}

void Particle::setRadius(double newRadius)
{
    m_radius = newRadius;
}

void Particle::setActive(bool value)
{
    m_active = value;
}

void Particle::move(double deltaTime)
{
    m_position += m_velocity * deltaTime;
}

void Particle::absorb(const Particle &other)
{
    const double combinedMass = m_mass + other.m_mass;
    const Vector2D combinedPosition =
        (m_position * m_mass + other.m_position * other.m_mass) / combinedMass;
    const Vector2D combinedVelocity =
        (m_velocity * m_mass + other.m_velocity * other.m_mass) / combinedMass;

    m_position = combinedPosition;
    m_velocity = combinedVelocity;
    m_mass = combinedMass;
    m_radius = std::sqrt(m_radius * m_radius + other.m_radius * other.m_radius);
}
