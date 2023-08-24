#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QCheckBox>

#include "LayerSelectWidget.hpp"

BrushIconWidget::BrushIconWidget(QColor t_brushColor, Qt::BrushStyle t_brushStyle, QWidget* t_parent)
    : brushColor(t_brushColor)
    , brushStyle(t_brushStyle)
    , QWidget(t_parent) {};

void BrushIconWidget::paintEvent(QPaintEvent* t_event)
{
    Q_UNUSED(t_event);

    QPainter* painter = new QPainter();

    painter->begin(this);
    painter->setBrush(QBrush(brushColor, brushStyle));
    painter->drawRect(QRect(0, 0, 20, 20));
    painter->end();
}

LayerCheckWidget::LayerCheckWidget(uint16_t t_number, QString t_text, QColor t_brushColor, Qt::BrushStyle t_brushStyle, QWidget* t_parent)
    : m_number(t_number)
    , QWidget(t_parent)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    QCheckBox* checkBox = new QCheckBox("", this);
    BrushIconWidget* brushIcon = new BrushIconWidget(t_brushColor, t_brushStyle, this);
    QLabel* label = new QLabel(t_text, this);

    checkBox->setCheckable(Qt::Checked);
    checkBox->setChecked(true);
    brushIcon->setMinimumSize(QSize(21, 21));
    label->setFont(QFont("Purisa", 9));

    hbox->addWidget(label, 0);
    hbox->addWidget(brushIcon, 0);
    hbox->addWidget(checkBox, 0);
    hbox->setSpacing(10);
    hbox->setContentsMargins(0, 5, 0, 5);

    connect(checkBox, &QCheckBox::toggled, [this](bool t_checked) { this->sendChecked(this->m_number, t_checked); });
    setLayout(hbox);
}

LayerSelectWidget::LayerSelectWidget(QWidget* t_parent)
    : QWidget(t_parent)
{
    QVBoxLayout* vbox = new QVBoxLayout(this);

    vbox->addStretch();

    for (auto& [first, second] : paintData) {
        LayerCheckWidget* checkBox = new LayerCheckWidget(first, second.name, second.brushColor, second.brushStyle, this);

        connect(checkBox, &LayerCheckWidget::sendChecked, [this](uint16_t m_number, bool t_checked) { this->sendLayer(m_number, t_checked); });

        vbox->addWidget(checkBox, 0, Qt::AlignRight | Qt::AlignVCenter);
    }

    vbox->addStretch();

    setLayout(vbox);
}