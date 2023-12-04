#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>

#include "Workers/TokenGenerator.hpp"
#include "LayoutCard.hpp"
#include "Include/LayoutReader.hpp"

LayoutCard::LayoutCard(
    const QString& Name,
    QWidget* Parent)
    : QDialog(Parent)
    , hashLbl(new QLabel)
    , token(TokenGenerator::GetNewToken())
    ,fName(Name)
    , layoutView(new LayoutWidget)
{
   setWindowTitle("Карточка файла топологии " + Name.mid(Name.lastIndexOf('/') + 1));
   setMinimumSize(800, 600);
   QVBoxLayout* layout = new QVBoxLayout(this);
   QSplitter* splitter = new QSplitter;
   layout->addWidget(splitter);
   splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   splitter->addWidget(CreateInfoWidget());
   splitter->addWidget(layoutView);

   emit layoutView->setFile(fName.toStdString());

}


QWidget*
LayoutCard::CreateInfoWidget()
{
    QGroupBox* ret = new QGroupBox("Общая информация");
    ret->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* mainLayout = new QVBoxLayout(ret);
    auto packWidgets = [](const QString& Label, QWidget* InfoW, QPushButton* Btn = nullptr) -> QLayout*
        {
            QLabel* lbl = new QLabel(Label);

            lbl->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            InfoW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            if(Btn) { Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); }

            QHBoxLayout* layout = new QHBoxLayout;
            layout->addWidget(lbl);
            layout->addWidget(InfoW);
            if(Btn) { layout->addWidget(Btn);}
            return layout;
        };

   //hash
   QPushButton* recalcHashBtn = new QPushButton("Пересчитать");
   connect(recalcHashBtn, &QPushButton::clicked, this, [this](){ emit AskForCrc(token, fName);});

   mainLayout->addLayout(packWidgets("CRC32 хеш:", hashLbl, recalcHashBtn));

   mainLayout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding));
   return ret;
}