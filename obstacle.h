#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QRectF>

class Obstacle
{
public:
    Obstacle(int obstacleId = 0, const QRectF &rectangle = QRectF(), double restitution = 0.65);

    int id() const;
    QRectF rect() const;
    double restitution() const;

private:
    int m_id;
    QRectF m_rect;
    double m_restitution;
};

#endif
