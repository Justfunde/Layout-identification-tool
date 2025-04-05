#ifndef GEOMETRY_WIDGET_H
#define GEOMETRY_WIDGET_H

#include <QMouseEvent>
#include <QObject>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QWidget>

#include "Include/LayoutData.hpp"
#include "PaintData.hpp"

/**
 * @brief Handle scale data for topology
 *
 */
struct Scale {
    uint32_t initialW {}; // original with of topology
    uint32_t initialH {}; // original height of topology
    double initial { 1.0 }; // inital scaling -> set after each resize
    double current { 1.0 }; // current scaling -> changes after zoom in/out
    double scroll {};
};

/**
 * @brief Handle mouse data
 *
 */
struct Mouse {
    QPoint triggerPos {}; // Position of mouse in trigger(press) moment;
};

/**
 * @brief Handle axes data
 *
 */
struct Axes {
    QPoint pos {}; // current position of axes
    QPoint moveIn {}; // position to move in axes -> changes by custom drag event
};

/**
 * @brief Paint all geometry in topology...
 *
 */
class GeometryWidget : public QWidget {
    Q_OBJECT

    lds::LayoutData* layout {}; // layout data from LayoutReader
    lds::Coord min { std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max() }; // Top left corner of topology in 2D space
    lds::Coord max { 0, 0 }; // Bottom right corner of topology in 2D space
    Scale m_scale {};
    Mouse m_mouse {};
    Axes m_axes {};

public:
    GeometryWidget(QWidget* t_parent = nullptr);

protected:
    void paintEvent(QPaintEvent* t_event);
    void resizeEvent(QResizeEvent* t_event);
    void wheelEvent(QWheelEvent* t_event);
    void mousePressEvent(QMouseEvent* t_event);
    void mouseMoveEvent(QMouseEvent* t_event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    /**
     * @brief Paint a grid
     *
     * @param t_painter qt painter...
     */
    void paintGrid(QPainter* t_painter);

    /**
     * @brief Paint path
     *
     * @param t_path path data
     * @param t_painter qt painter...
     */
    void paintPath(lds::Path* t_path, QPainter* t_painter);

    /**
     * @brief Paint polygon
     *
     * @param t_polygon polygon data
     * @param t_painter qt painter...
     */
    void paintPolygon(lds::Polygon* t_polygon, QPainter* t_painter);

    /**
     * @brief Paint rectangle
     *
     * @param t_rectangle rectangle data
     * @param t_painter qt painter...
     */
    void paintRectangle(lds::Rectangle* t_rectangle, QPainter* t_painter);

    /**
     * @brief Paint reference -> all geometry in this reference
     *
     * @param t_reference reference with geometry data
     * @param t_painter qt painter
     */
    void paintReference(lds::Reference* t_reference, QPainter* t_painter);

public slots:
    /**
     * @brief Update layers visibility on incoming signal
     *
     * @param t_key layer's ley
     * @param t_isChecked checked state
     */
    void updateLayer(const uint16_t& t_key, const bool& t_isChecked);

    void setLayout(lds::LayoutData* t_layout);
};

#endif