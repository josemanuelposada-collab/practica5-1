#include "trajectorywidget.h"

#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <algorithm>

TrajectoryWidget::TrajectoryWidget(const Simulation &simulation, QWidget *parent)
    : QWidget(parent),
      m_simulation(simulation),
      m_currentFrame(0)
{
    setWindowTitle("Actividad 1 - Trayectorias de particulas");
    resize(1000, 700);

    connect(&m_timer, &QTimer::timeout, this, &TrajectoryWidget::advanceAnimation);
    m_timer.start(16);
}

bool TrajectoryWidget::saveImage(const QString &filePath, const QSize &imageSize)
{
    QImage image(imageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor("#f8f9fa"));

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawSimulation(painter,
                   QRectF(50.0, 78.0, imageSize.width() - 100.0, imageSize.height() - 160.0),
                   maxFrameCount() - 1);
    return image.save(filePath);
}

void TrajectoryWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor("#f8f9fa"));
    drawSimulation(painter, QRectF(45.0, 76.0, width() - 90.0, height() - 145.0), m_currentFrame);
}

QPointF TrajectoryWidget::toScreen(const Vector2D &point, const QRectF &canvas) const
{
    const QRectF bounds = m_simulation.bounds();
    const double xScale = canvas.width() / bounds.width();
    const double yScale = canvas.height() / bounds.height();

    return QPointF(canvas.left() + (point.x - bounds.left()) * xScale,
                   canvas.top() + (point.y - bounds.top()) * yScale);
}

void TrajectoryWidget::drawSimulation(QPainter &painter, const QRectF &canvas, int frame)
{
    const QRectF bounds = m_simulation.bounds();
    const double xScale = canvas.width() / bounds.width();
    const double yScale = canvas.height() / bounds.height();

    drawHeader(painter, canvas);

    painter.setPen(QPen(QColor("#1f2937"), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(canvas);

    painter.setPen(QPen(QColor("#94a3b8"), 1, Qt::DashLine));
    for (int x = 100; x < bounds.width(); x += 100) {
        const double screenX = canvas.left() + x * xScale;
        painter.drawLine(QPointF(screenX, canvas.top()), QPointF(screenX, canvas.bottom()));
    }
    for (int y = 100; y < bounds.height(); y += 100) {
        const double screenY = canvas.top() + y * yScale;
        painter.drawLine(QPointF(canvas.left(), screenY), QPointF(canvas.right(), screenY));
    }

    painter.setPen(QPen(QColor("#111827"), 1));
    painter.setBrush(QColor("#4b5563"));
    for (const Obstacle &obstacle : m_simulation.obstacles()) {
        const QRectF rect = obstacle.rect();
        QRectF screenRect(canvas.left() + rect.left() * xScale,
                          canvas.top() + rect.top() * yScale,
                          rect.width() * xScale,
                          rect.height() * yScale);
        painter.drawRect(screenRect);
    }

    const QVector<Particle> particles = m_simulation.particles();
    const QMap<int, QVector<TrajectoryPoint>> trajectories = m_simulation.trajectories();

    for (const Particle &particle : particles) {
        const QVector<TrajectoryPoint> points = trajectories.value(particle.id());
        if (points.size() < 2) {
            continue;
        }

        const int lastIndex = std::min(frame, static_cast<int>(points.size()) - 1);
        if (lastIndex <= 0) {
            continue;
        }

        QPainterPath path;
        path.moveTo(toScreen(points.first().position, canvas));
        for (int i = 1; i <= lastIndex; ++i) {
            path.lineTo(toScreen(points[i].position, canvas));
        }

        painter.setPen(QPen(particle.color(), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);

        painter.setPen(QPen(QColor("#111827"), 1));
        painter.setBrush(particle.color());
        const QPointF currentPoint = toScreen(points[lastIndex].position, canvas);
        const double radius = std::max(4.0, points[lastIndex].radius * (xScale + yScale) * 0.5);
        painter.drawEllipse(currentPoint, radius, radius);
    }

    drawLegend(painter, canvas);
}

void TrajectoryWidget::drawHeader(QPainter &painter, const QRectF &canvas) const
{
    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(QColor("#111827"));
    painter.drawText(QRectF(canvas.left(), canvas.top() - 48.0, canvas.width(), 24.0),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "Trayectorias de particulas - Actividad 1");

    QFont detailFont = painter.font();
    detailFont.setPointSize(9);
    detailFont.setBold(false);
    painter.setFont(detailFont);
    painter.setPen(QColor("#4b5563"));
    painter.drawText(QRectF(canvas.left(), canvas.top() - 24.0, canvas.width(), 20.0),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QString("Obstaculos: %1 | Colisiones registradas: %2")
                         .arg(m_simulation.obstacles().size())
                         .arg(m_simulation.collisionEvents().size()));
}

void TrajectoryWidget::drawLegend(QPainter &painter, const QRectF &canvas) const
{
    const QVector<Particle> particles = m_simulation.particles();
    const double legendTop = canvas.bottom() + 18.0;
    const double itemWidth = 150.0;

    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    legendFont.setBold(false);
    painter.setFont(legendFont);

    for (int i = 0; i < particles.size(); ++i) {
        const Particle &particle = particles[i];
        const double x = canvas.left() + (i % 5) * itemWidth;
        const double y = legendTop + (i / 5) * 24.0;

        painter.setPen(QPen(QColor("#111827"), 1));
        painter.setBrush(particle.color());
        painter.drawEllipse(QPointF(x + 8.0, y + 8.0), 6.0, 6.0);

        painter.setPen(QColor("#111827"));
        painter.drawText(QRectF(x + 22.0, y, itemWidth - 24.0, 18.0),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         QString("Particula %1").arg(particle.id()));
    }
}

int TrajectoryWidget::maxFrameCount() const
{
    int maximum = 0;
    const QMap<int, QVector<TrajectoryPoint>> trajectories = m_simulation.trajectories();
    for (const QVector<TrajectoryPoint> &points : trajectories) {
        maximum = std::max(maximum, static_cast<int>(points.size()));
    }
    return maximum;
}

void TrajectoryWidget::advanceAnimation()
{
    const int maximum = maxFrameCount();
    if (maximum <= 1) {
        return;
    }

    m_currentFrame++;
    if (m_currentFrame >= maximum) {
        m_currentFrame = 0;
    }

    update();
}
