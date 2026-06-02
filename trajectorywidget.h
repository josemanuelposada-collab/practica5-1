#ifndef TRAJECTORYWIDGET_H
#define TRAJECTORYWIDGET_H

#include "simulation.h"

#include <QTimer>
#include <QWidget>

class TrajectoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrajectoryWidget(const Simulation &simulation, QWidget *parent = nullptr);

    bool saveImage(const QString &filePath, const QSize &imageSize = QSize(1000, 700));

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPointF toScreen(const Vector2D &point, const QRectF &canvas) const;
    void drawSimulation(QPainter &painter, const QRectF &canvas, int frame);
    int maxFrameCount() const;
    void advanceAnimation();

    Simulation m_simulation;
    QTimer m_timer;
    int m_currentFrame;
};

#endif
