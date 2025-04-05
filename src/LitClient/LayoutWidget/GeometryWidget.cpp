#include <QGuiApplication>
#include <QPainter>
#include <QTimeLine>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

#include "GeometryWidget.hpp"

#define __PIXEL_SIZE__ 20
#define __BORDERS__ 0.95
#define __MOVE_FACTOR__ 0.35
#define __GRID_SMALL_STEP__ 1
#define __GRID_BIG_STEP__ 10

GeometryWidget::GeometryWidget(QWidget* t_parent)
    : QWidget(t_parent)
{
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(16, 16, 16));
    setAutoFillBackground(true);
    setPalette(pal);
};

void GeometryWidget::setLayout(lds::LayoutData* t_layout)
{
    if (layout != nullptr) {
        min = { std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max() };
        max = { 0, 0 };

        m_scale.current = 1.0;
        m_scale.initial = 1.0;
        m_scale.scroll = 0.0;

        m_axes.pos = { 0, 0 };
        m_axes.moveIn = { 0, 0 };

        m_mouse.triggerPos = { 0, 0 };

        for (auto& [first, second] : paintData) {
            second.isActive = true;
        }
    }

    layout = t_layout;

    if (layout->libraries.size() != 0) {
        for (auto& layer : layout->libraries[0]->layers) {
            for (auto& geom : layer.geometries) {
                for (auto& coord : geom->coords) {
                    min.x = std::min(coord.x, min.x);
                    min.y = std::min(coord.y, min.y);
                    max.x = std::max(coord.x, max.x);
                    max.y = std::max(coord.y, max.y);
                }
            }
        }

        if (min.x != 0 || min.y != 0) {
            for (auto& layer : layout->libraries[0]->layers) {
                for (auto& geom : layer.geometries) {
                    for (auto& coord : geom->coords) {
                        coord.x = (coord.x - min.x - (max.x - min.x) / 2.0);
                        coord.y = (coord.y - min.y - (max.y - min.y) / 2.0);
                    }
                }
            }
        }

        std::sort(layout->libraries[0]->layers.begin(), layout->libraries[0]->layers.end(), [](const lds::Layer& t_l1, const lds::Layer& t_l2) {
            return t_l1.layer < t_l2.layer;
        });
    }

    double newInitial = std::min((width() * __BORDERS__) / (max.x - min.x), (height() * __BORDERS__) / (max.y - min.y));

    m_scale.current = m_scale.current / m_scale.initial * newInitial;
    m_scale.initial = newInitial;

    update();
}

void GeometryWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);

    painter->translate(width() / 2.0, height() / 2.0);
    painter->scale(m_scale.current / __PIXEL_SIZE__, m_scale.current / __PIXEL_SIZE__);

    paintGrid(painter);

    if (layout != nullptr) {
        for (auto& layer : layout->libraries[0]->layers) {
            if (paintData.count(layer.layer) != 0 && paintData.at(layer.layer).isActive) {
                painter->setPen(QPen(paintData.at(layer.layer).penColor, 1.0 / m_scale.current * __PIXEL_SIZE__, Qt::SolidLine, Qt::RoundCap));
                painter->setBrush(QBrush(paintData.at(layer.layer).brushColor, paintData.at(layer.layer).brushStyle));

                for (auto& geom : layer.geometries) {
                    switch (geom->type) {

                    case lds::GeometryType::path: {
                        auto path = static_cast<lds::Path*>(geom);
                        paintPath(path, painter);
                        break;
                    }

                    case lds::GeometryType::polygon: {
                        auto polygon = static_cast<lds::Polygon*>(geom);
                        paintPolygon(polygon, painter);
                        break;
                    }

                    case lds::GeometryType::rectangle: {
                        auto rectangle = static_cast<lds::Rectangle*>(geom);
                        paintRectangle(rectangle, painter);
                        break;
                    }

                    case lds::GeometryType::reference: {
                        auto reference = static_cast<lds::Reference*>(geom);
                        paintReference(reference, painter);
                        break;
                    }

                    default:
                        break;
                    }
                }
            } else {
                painter->setBrush(QBrush(QColor(0, 0, 0, 0)));
                painter->setPen(QPen(QColor(0, 0, 0, 0)));
            }
        }
    }

    painter->end();
}

void GeometryWidget::resizeEvent(QResizeEvent* t_event)
{
    Q_UNUSED(t_event);

    if (layout != nullptr) {
        double newInitial = std::min((width() * __BORDERS__) / (max.x - min.x), (height() * __BORDERS__) / (max.y - min.y));

        m_scale.current = m_scale.current / m_scale.initial * newInitial;
        m_scale.initial = newInitial;
    }
}

void GeometryWidget::wheelEvent(QWheelEvent* t_event)
{
    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {
        double sigmoid = 1.0 / (1 + std::pow(2.718281282846, -1 * m_scale.scroll));

        if ((sigmoid > 0.5 || t_event->angleDelta().y() > 0) && (sigmoid < 0.995 || t_event->angleDelta().y() < 0)) {
            m_scale.scroll += 0.1 * (t_event->angleDelta().y() > 0 ? 1 : -1);
            m_scale.current = m_scale.initial / (1.0 / (std::pow(2.718281282846, 1 * m_scale.scroll)));
        }

        update();
    }
}

void GeometryWidget::mousePressEvent(QMouseEvent* t_event)
{
    if (t_event->button() == Qt::LeftButton) {
        m_mouse.triggerPos = t_event->pos();

        QCursor changedCursor = cursor();
        changedCursor.setShape(Qt::ClosedHandCursor);
        setCursor(changedCursor);
    }
}

void GeometryWidget::mouseMoveEvent(QMouseEvent* t_event)
{
    if (t_event->buttons() & Qt::LeftButton) {
        m_axes.moveIn = m_axes.pos - (m_mouse.triggerPos - t_event->pos()) * __MOVE_FACTOR__ / m_scale.current * __PIXEL_SIZE__;

        update();
    }
}

void GeometryWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_axes.pos = m_axes.moveIn;

    QCursor changedCursor = cursor();
    changedCursor.setShape(Qt::ArrowCursor);
    setCursor(changedCursor);
}

void GeometryWidget::paintGrid(QPainter* t_painter)
{
    double cameraWidth = (width() / m_scale.current);
    double cameraHeight = (height() / m_scale.current);

    double __width = std::max(static_cast<double>(width()), cameraWidth);
    double __height = std::max(static_cast<double>(height()), cameraHeight);

    uint32_t stepScale = m_scale.current < 5.0 ? 5 : 1;

    double maxXLines = __width / (__GRID_SMALL_STEP__ * stepScale);
    double maxYLines = __height / (__GRID_SMALL_STEP__ * stepScale);

    for (size_t i {}; i < maxYLines; ++i) {
        double y = (__GRID_SMALL_STEP__ * stepScale) * i - __height / 2.0;

        if (y >= -cameraHeight / 2.0 && y <= cameraHeight / 2.0) {

            if ((i + 1) % (__GRID_BIG_STEP__ * stepScale) == 0) {
                t_painter->setPen(QPen(QColor(64, 64, 64), 1 / m_scale.current * __PIXEL_SIZE__, Qt::SolidLine, Qt::RoundCap));
            } else {
                t_painter->setPen(QPen(QColor(32, 32, 32), 1 / m_scale.current * __PIXEL_SIZE__, Qt::SolidLine, Qt::RoundCap));
            }

            t_painter->drawLine(QPoint(-cameraWidth / 2.0, y) * __PIXEL_SIZE__, QPoint(cameraWidth / 2.0, y) * __PIXEL_SIZE__);
        }
    }

    for (size_t i {}; i < maxXLines; ++i) {
        double x = (__GRID_SMALL_STEP__ * stepScale) * i - __width / 2.0;

        if (x >= -cameraWidth / 2.0 && x <= cameraWidth / 2.0) {

            if ((i + 1) % (__GRID_BIG_STEP__ * stepScale) == 0) {
                t_painter->setPen(QPen(QColor(64, 64, 64), 1 / m_scale.current * __PIXEL_SIZE__, Qt::SolidLine, Qt::RoundCap));
            } else {
                t_painter->setPen(QPen(QColor(32, 32, 32), 1 / m_scale.current * __PIXEL_SIZE__, Qt::SolidLine, Qt::RoundCap));
            }

            t_painter->drawLine(QPoint(x, -cameraHeight / 2.0) * __PIXEL_SIZE__, QPoint(x, cameraHeight / 2.0) * __PIXEL_SIZE__);
        }
    }
}

void GeometryWidget::paintPath(lds::Path* t_path, QPainter* t_painter)
{
    uint8_t halfWidth = t_path->width / 2.0;
    std::vector<lds::Coord> bottom { t_path->coords };
    std::vector<lds::Coord> top { t_path->coords };

    for (size_t i {}; i < t_path->coords.size() - 1; ++i) {
        if (t_path->coords[i].x == t_path->coords[i + 1].x) {
            bottom[i].y -= halfWidth;
            bottom[i + 1].y -= halfWidth;

            top[i].y += halfWidth;
            top[i + 1].y += halfWidth;
        } else {
            bottom[i].x += halfWidth;
            bottom[i + 1].x += halfWidth;

            top[i].x -= halfWidth;
            top[i + 1].x -= halfWidth;
        }
    }

    QPolygon polygon {};

    bottom.insert(bottom.end(), top.rbegin(), top.rend());
    bottom.insert(bottom.end(), bottom.at(0));

    for (auto& b : bottom) {
        polygon.append(QPoint(b.x, b.y) * __PIXEL_SIZE__ + m_axes.moveIn);
    }

    t_painter->drawPolygon(polygon);
}

void GeometryWidget::paintPolygon(lds::Polygon* t_polygon, QPainter* t_painter)
{
    QPolygon polygon {};

    for (auto& coord : t_polygon->coords) {
        polygon.append(QPoint(coord.x, coord.y) * __PIXEL_SIZE__ + m_axes.moveIn);
    }

    t_painter->drawPolygon(polygon);
}

void GeometryWidget::paintRectangle(lds::Rectangle* t_rectangle, QPainter* t_painter)
{
    QPolygon polygon {};

    for (auto& coord : t_rectangle->coords) {
        polygon.append(QPoint(coord.x, coord.y) * __PIXEL_SIZE__ + m_axes.moveIn);
    }

    t_painter->drawPolygon(polygon);
}

void GeometryWidget::paintReference(lds::Reference* t_reference, QPainter* t_painter)
{
    QPolygon polygon {};

    for (auto& geom : t_reference->pElement->geometries) {
        switch (geom->type) {

        case lds::GeometryType::path: {
            auto path = static_cast<lds::Path*>(geom);
            paintPath(path, t_painter);
            break;
        }

        case lds::GeometryType::polygon: {
            auto polygon = static_cast<lds::Polygon*>(geom);
            paintPolygon(polygon, t_painter);
            break;
        }

        case lds::GeometryType::rectangle: {
            auto rectangle = static_cast<lds::Rectangle*>(geom);
            paintRectangle(rectangle, t_painter);
            break;
        }

        case lds::GeometryType::reference: {
            auto reference = static_cast<lds::Reference*>(geom);
            paintReference(reference, t_painter);
            break;
        }

        default:
            break;
        }
    }
}

void GeometryWidget::updateLayer(const uint16_t& t_key, const bool& t_isChecked)
{
    paintData.at(t_key).isActive = t_isChecked;

    update();
}