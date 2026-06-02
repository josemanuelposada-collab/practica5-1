#include "obstacle.h"

Obstacle::Obstacle(int obstacleId, const QRectF &rectangle, double restitution)
    : m_id(obstacleId), m_rect(rectangle), m_restitution(restitution)
{
}

int Obstacle::id() const
{
    return m_id;
}

QRectF Obstacle::rect() const
{
    return m_rect;
}

double Obstacle::restitution() const
{
    return m_restitution;
}
