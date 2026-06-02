#include "simulation.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <cmath>

Simulation::Simulation()
    : m_bounds(0.0, 0.0, 800.0, 500.0),
      m_deltaTime(0.016),
      m_totalTime(24.0),
      m_currentTime(0.0)
{
}

void Simulation::setupDefaultScenario()
{
    m_particles.clear();
    m_obstacles.clear();
    m_trajectories.clear();
    m_collisionEvents.clear();
    m_currentTime = 0.0;

    m_particles.append(Particle(1, Vector2D(90.0, 250.0), Vector2D(75.0, 0.0), 2.0, 10.0, QColor("#e63946")));
    m_particles.append(Particle(2, Vector2D(700.0, 250.0), Vector2D(-55.0, 0.0), 1.5, 9.0, QColor("#2a9d8f")));
    m_particles.append(Particle(3, Vector2D(115.0, 405.0), Vector2D(61.0, -46.0), 2.5, 11.0, QColor("#457b9d")));
    m_particles.append(Particle(4, Vector2D(700.0, 385.0), Vector2D(-63.0, -43.0), 1.8, 9.5, QColor("#f4a261")));
    m_particles.append(Particle(5, Vector2D(395.0, 260.0), Vector2D(34.0, -58.0), 1.3, 8.5, QColor("#7b2cbf")));

    m_obstacles.append(Obstacle(1, QRectF(230.0, 125.0, 58.0, 58.0), 0.78));
    m_obstacles.append(Obstacle(2, QRectF(515.0, 110.0, 66.0, 66.0), 0.80));
    m_obstacles.append(Obstacle(3, QRectF(190.0, 315.0, 70.0, 70.0), 0.76));
    m_obstacles.append(Obstacle(4, QRectF(545.0, 305.0, 62.0, 62.0), 0.79));

    saveCurrentPositions();
}

void Simulation::run()
{
    while (m_currentTime < m_totalTime) {
        step();
    }
}

bool Simulation::exportTextFiles(const QString &directoryPath) const
{
    QDir dir(directoryPath);
    if (!dir.exists() && !dir.mkpath(".")) {
        return false;
    }

    QFile trajectoryFile(dir.filePath("trayectorias.csv"));
    if (!trajectoryFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream trajectoryOut(&trajectoryFile);
    trajectoryOut << "tiempo,id_particula,x,y,vx,vy,masa,radio\n";
    for (const Particle &particle : m_particles) {
        const QVector<TrajectoryPoint> points = m_trajectories.value(particle.id());
        for (const TrajectoryPoint &point : points) {
            trajectoryOut << QString::number(point.time, 'f', 3) << ","
                          << particle.id() << ","
                          << QString::number(point.position.x, 'f', 3) << ","
                          << QString::number(point.position.y, 'f', 3) << ","
                          << QString::number(point.velocity.x, 'f', 3) << ","
                          << QString::number(point.velocity.y, 'f', 3) << ","
                          << QString::number(point.mass, 'f', 3) << ","
                          << QString::number(point.radius, 'f', 3) << "\n";
        }
    }

    QFile collisionFile(dir.filePath("colisiones.txt"));
    if (!collisionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream collisionOut(&collisionFile);
    collisionOut << "Registro de colisiones\n";
    collisionOut << "tiempo | evento\n";
    for (const CollisionEvent &event : m_collisionEvents) {
        collisionOut << QString::number(event.time, 'f', 3) << " | " << event.description << "\n";
    }

    return true;
}

QRectF Simulation::bounds() const
{
    return m_bounds;
}

QVector<Particle> Simulation::particles() const
{
    return m_particles;
}

QVector<Obstacle> Simulation::obstacles() const
{
    return m_obstacles;
}

QMap<int, QVector<TrajectoryPoint>> Simulation::trajectories() const
{
    return m_trajectories;
}

QVector<CollisionEvent> Simulation::collisionEvents() const
{
    return m_collisionEvents;
}

void Simulation::step()
{
    m_currentTime += m_deltaTime;

    for (Particle &particle : m_particles) {
        if (!particle.isActive()) {
            continue;
        }

        particle.move(m_deltaTime);
        resolveWallCollisions(particle);
        resolveObstacleCollisions(particle);
    }

    resolveParticleCollisions();
    saveCurrentPositions();
}

void Simulation::saveCurrentPositions()
{
    for (const Particle &particle : m_particles) {
        if (!particle.isActive()) {
            continue;
        }

        m_trajectories[particle.id()].append({m_currentTime,
                                              particle.position(),
                                              particle.velocity(),
                                              particle.mass(),
                                              particle.radius()});
    }
}

void Simulation::resolveWallCollisions(Particle &particle)
{
    Vector2D position = particle.position();
    Vector2D velocity = particle.velocity();
    bool collided = false;

    if (position.x - particle.radius() < m_bounds.left()) {
        position.x = m_bounds.left() + particle.radius();
        velocity.x = std::abs(velocity.x);
        collided = true;
    } else if (position.x + particle.radius() > m_bounds.right()) {
        position.x = m_bounds.right() - particle.radius();
        velocity.x = -std::abs(velocity.x);
        collided = true;
    }

    if (position.y - particle.radius() < m_bounds.top()) {
        position.y = m_bounds.top() + particle.radius();
        velocity.y = std::abs(velocity.y);
        collided = true;
    } else if (position.y + particle.radius() > m_bounds.bottom()) {
        position.y = m_bounds.bottom() - particle.radius();
        velocity.y = -std::abs(velocity.y);
        collided = true;
    }

    if (collided) {
        particle.setPosition(position);
        particle.setVelocity(velocity);
        logCollision(QString("Particula %1 reboto elasticamente contra una pared").arg(particle.id()));
    }
}

void Simulation::resolveObstacleCollisions(Particle &particle)
{
    for (const Obstacle &obstacle : m_obstacles) {
        Vector2D normal;
        double penetration = 0.0;
        if (!circleIntersectsRect(particle, obstacle.rect(), &normal, &penetration)) {
            continue;
        }

        Vector2D velocity = particle.velocity();
        const double normalSpeed = velocity.dot(normal);
        if (normalSpeed < 0.0) {
            velocity = velocity - normal * ((1.0 + obstacle.restitution()) * normalSpeed);
            particle.setVelocity(velocity);
            particle.setPosition(particle.position() + normal * (penetration + 0.05));
            logCollision(QString("Particula %1 choco inelasticamente con obstaculo %2 (e=%3)")
                             .arg(particle.id())
                             .arg(obstacle.id())
                             .arg(obstacle.restitution(), 0, 'f', 2));
        } else {
            particle.setPosition(particle.position() + normal * (penetration + 0.05));
        }
    }
}

void Simulation::resolveParticleCollisions()
{
    for (int i = 0; i < m_particles.size(); ++i) {
        if (!m_particles[i].isActive()) {
            continue;
        }

        for (int j = i + 1; j < m_particles.size(); ++j) {
            if (!m_particles[j].isActive()) {
                continue;
            }

            const Vector2D difference = m_particles[j].position() - m_particles[i].position();
            const double minimumDistance = m_particles[i].radius() + m_particles[j].radius();
            if (difference.lengthSquared() > minimumDistance * minimumDistance) {
                continue;
            }

            const Vector2D relativeVelocity = m_particles[j].velocity() - m_particles[i].velocity();
            if (relativeVelocity.dot(difference) >= 0.0) {
                continue;
            }

            const Vector2D normal = difference.normalized();
            const double penetration = minimumDistance - difference.length();
            if (!qFuzzyIsNull(normal.lengthSquared())) {
                m_particles[i].setPosition(m_particles[i].position() - normal * (penetration * 0.5));
                m_particles[j].setPosition(m_particles[j].position() + normal * (penetration * 0.5));
            }

            const int absorbedId = m_particles[j].id();
            m_particles[i].absorb(m_particles[j]);
            m_particles[j].setActive(false);
            logCollision(QString("Particulas %1 y %2 se fusionaron completamente inelasticamente")
                             .arg(m_particles[i].id())
                             .arg(absorbedId));
        }
    }
}

bool Simulation::circleIntersectsRect(const Particle &particle,
                                      const QRectF &rect,
                                      Vector2D *normal,
                                      double *penetration) const
{
    const Vector2D center = particle.position();
    const double closestX = std::clamp(center.x, rect.left(), rect.right());
    const double closestY = std::clamp(center.y, rect.top(), rect.bottom());
    Vector2D fromClosest(center.x - closestX, center.y - closestY);
    double distance = fromClosest.length();

    if (distance > particle.radius()) {
        return false;
    }

    if (!qFuzzyIsNull(distance)) {
        *normal = fromClosest / distance;
        *penetration = particle.radius() - distance;
        return true;
    }

    const double exitLeft = std::abs(center.x - rect.left());
    const double exitRight = std::abs(rect.right() - center.x);
    const double exitTop = std::abs(center.y - rect.top());
    const double exitBottom = std::abs(rect.bottom() - center.y);
    const double minExit = std::min(std::min(exitLeft, exitRight), std::min(exitTop, exitBottom));

    if (minExit == exitLeft) {
        *normal = Vector2D(-1.0, 0.0);
    } else if (minExit == exitRight) {
        *normal = Vector2D(1.0, 0.0);
    } else if (minExit == exitTop) {
        *normal = Vector2D(0.0, -1.0);
    } else {
        *normal = Vector2D(0.0, 1.0);
    }
    *penetration = particle.radius() + minExit;
    return true;
}

void Simulation::logCollision(const QString &description)
{
    m_collisionEvents.append({m_currentTime, description});
}
