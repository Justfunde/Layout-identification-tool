#ifndef LAYER_SELECT_WIDGET_H
#define LAYER_SELECT_WIDGET_H

#include <QString>
#include <QWidget>

#include "PaintData.hpp"

/**
 * @brief Custom widget to display layer's filling brush.
 *
 */
class BrushIconWidget : public QWidget {
    Q_OBJECT

    QColor brushColor {};
    Qt::BrushStyle brushStyle {};

public:
    BrushIconWidget(QColor t_brushColor, Qt::BrushStyle t_brushStyle, QWidget* t_parent = nullptr);

protected:
    void paintEvent(QPaintEvent* t_event);
};

/**
 * @brief Custom widget that combines checkbox, BrushIconWidget and layer's label in one.
 *
 */
class LayerCheckWidget : public QWidget {
    Q_OBJECT

    uint16_t m_number {}; // layer's key(number)

public:
    LayerCheckWidget(uint16_t t_number, QString t_text, QColor t_brushColor, Qt::BrushStyle t_brushStyle, QWidget* t_parent = nullptr);

public slots:
signals:
    /**
     * @brief Signal thats send key and state of checked/unchecked layer
     *
     * @param t_key layer's key
     * @param t_isChecked state of checking
     */
    void sendChecked(const uint16_t& t_key, const bool& t_isChecked);
};

/**
 * @brief Custom widget that combines all LayerCheckWidget together.
 *
 */
class LayerSelectWidget : public QWidget {
    Q_OBJECT

public:
    LayerSelectWidget(QWidget* t_parent = nullptr);

public slots:
signals:
    /**
     * @brief Signal thats send key and state of checked/unchecked layer
     *
     * @param t_key layer's key
     * @param t_isChecked state of checking
     */
    void sendLayer(const uint16_t& t_key, const bool& t_isChecked);
};

#endif