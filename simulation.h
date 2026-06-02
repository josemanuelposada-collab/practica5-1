#ifndef SIMULATION_H
#define SIMULATION_H

#include "obstacle.h"
#include "particle.h"

#include <QMap>
#include <QRectF>
#include <QString>
#include <QVector>

struct TrajectoryPoint
{
    double time;
    Vector2D position;
    Vector2D velocity;
    double mass;
    double radius;
};

struct CollisionEvent
{
    double time;
    QString description;
};

class Simulation
{
public:
    Simulation();

    void setupDefaultScenario();
    void run();
    bool exportTextFiles(const QString &directoryPath) const;

    QRectF bounds() const;
    QVector<Particle> particles() const;
    QVector<Obstacle> obstacles() const;
    QMap<int, QVector<TrajectoryPoint>> trajectories() const;
    QVector<CollisionEvent> collisionEvents() const;

private:
    void step();
    void saveCurrentPositions();
    void resolveWallCollisions(Particle &particle);
    void resolveObstacleCollisions(Particle &particle);
    void resolveParticleCollisions();
    bool circleIntersectsRect(const Particle &particle, const QRectF &rect, Vector2D *normal, double *penetration) const;
    void logCollision(const QString &description);

    QRectF m_bounds;
    QVector<Particle> m_particles;
    QVector<Obstacle> m_obstacles;
    QMap<int, QVector<TrajectoryPoint>> m_trajectories;
    QVector<CollisionEvent> m_collisionEvents;
    double m_deltaTime;
    double m_totalTime;
    double m_currentTime;
};

#endif
