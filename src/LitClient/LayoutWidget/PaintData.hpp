#ifndef PAINT_DATA_H
#define PAINT_DATA_H

#include <QBrush>
#include <QPen>
#include <QString>
#include <map>

/**
 * @brief Paint data for layers
 *
 */
struct PaintData {
    QString name {}; // layer name
    QColor penColor {};
    QColor brushColor {};
    Qt::BrushStyle brushStyle {};
    bool isActive {}; // is layer should be displayed

    PaintData(QString t_name, QColor t_penColor, QColor t_brushColor, Qt::BrushStyle t_brushStyle = Qt::SolidPattern, bool t_isActive = true)
        : name(t_name)
        , penColor(t_penColor)
        , brushColor(t_brushColor)
        , brushStyle(t_brushStyle)
        , isActive(t_isActive) {};
};

// container of paint data for each of layers -> used in GeometryWidget and LayerSelectWidget
static std::map<uint16_t, PaintData> paintData {
    { 1, PaintData("N well", QColor(0, 94, 0), QColor(0, 94, 0, 125), Qt::Dense4Pattern) },
    { 16, PaintData("diffusion N", QColor(0, 94, 0), QColor(0, 94, 0, 125)) },
    { 17, PaintData("diffusion P", QColor(120, 112, 0), QColor(120, 112, 0, 125)) },
    { 13, PaintData("polysilicon", QColor(238, 20, 0), QColor(238, 20, 0, 125)) },
    { 19, PaintData("contact", QColor(255, 255, 255), QColor(255, 255, 255, 125)) },
    { 23, PaintData("metal", QColor(0, 47, 240), QColor(0, 47, 240, 125), Qt::Dense6Pattern) },
    { 27, PaintData("metal2", QColor(0, 9, 168), QColor(0, 9, 168, 125)) },
    { 34, PaintData("metal3", QColor(56, 60, 113), QColor(56, 60, 113, 125), Qt::BDiagPattern) },
    { 36, PaintData("metal4", QColor(102, 155, 3), QColor(102, 155, 3, 125), Qt::FDiagPattern) },
    { 53, PaintData("metal5", QColor(76, 73, 134), QColor(76, 73, 134, 125), Qt::HorPattern) },
    { 55, PaintData("metal6", QColor(127, 127, 127), QColor(127, 127, 127, 125), Qt::DiagCrossPattern) },
};

#endif