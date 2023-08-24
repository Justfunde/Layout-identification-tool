#include <QHBoxLayout>

#include "GeometryWidget.hpp"
#include "LayerSelectWidget.hpp"
#include "Include/LayoutReader.hpp"
#include "LayoutWidget.hpp"

LayoutWidget::LayoutWidget(QWidget* t_parent)
    : QWidget(t_parent)
{

    QHBoxLayout* hbox = new QHBoxLayout(this);

    GeometryWidget* geometryWidget = new GeometryWidget(this);
    LayerSelectWidget* layerSelectWidget = new LayerSelectWidget(this);


    connect(layerSelectWidget, &LayerSelectWidget::sendLayer, geometryWidget, &GeometryWidget::updateLayer);
    connect(this, &LayoutWidget::setFile, [geometryWidget](const std::string_view t_fileName) {
        lds::LayoutData* layout = new lds::LayoutData();
        LayoutReader* p_reader = GetReader(std::wstring(t_fileName.begin(), t_fileName.end()));
        p_reader->Read(layout);

        geometryWidget->setLayout(layout);
    });

    geometryWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layerSelectWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    geometryWidget->setContentsMargins(0, 0, 0, 0);
    layerSelectWidget->setContentsMargins(0, 0, 0, 0);

    hbox->addWidget(geometryWidget);
    hbox->addWidget(layerSelectWidget);
    hbox->setContentsMargins(0, 0, 0, 0);

    setLayout(hbox);
}