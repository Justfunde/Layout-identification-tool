#include "Headers/SettingsDialog.hpp"
#include "Headers/SettingsPages.hpp"

#include <QToolBar>
#include <QPushButton>

#include <array>
#include <string_view>
#include <QString>


namespace SettingParameters
{
    namespace ToolBarStrings
    {
        const QString pageNames[SettingsDialog::SettingPageId::cnt] = { 
            "Общее"/*,
            "Алгоритм анализа имен ячеек",
            "Алгоритм сравнения площадей ячеек",
            "Алгоритм анализа числа полигонов и распределения точек в ячейках",
            "Алгоритм анализа взаимного расположения ячеек",
            "Алгоритм поиска полигонов методом грубой силы",
            "Алгоритм детального анализа" */};
    }
}


SettingsDialog::SettingsDialog(
    QWidget* Parent )
    : QDialog(Parent)
    , tb(new QToolBar)
    , settingPages(new QStackedWidget) 
    , pages(CreateSettingPagesArray())
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    tb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    settingPages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPushButton* okBtn        = new QPushButton("ОК");
    QPushButton* applyBtn     = new QPushButton("Применить");
    QPushButton* cancelButton = new QPushButton("Отменить");
    okBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    applyBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    cancelButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum));
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(cancelButton);

    Init();
    mainLayout->addWidget(tb);
    mainLayout->addWidget(settingPages);
    mainLayout->addLayout(buttonLayout);
    setMinimumSize(300,200);

    connect(okBtn, &QPushButton::clicked, this, &SettingsDialog::OnOk);
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::OnApply);
    connect(cancelButton, &QPushButton::clicked, [this](){ reject();});
    connect(settingPages, &QStackedWidget::currentChanged,[this](int32_t i) {  pages[i]->InitPage(); });
    pages[0]->InitPage();
}

void
SettingsDialog::Init()
{
    tb->setStyleSheet("QToolBar {border-bottom: 2px solid black; border-top: 2px solid black;}");

    using namespace SettingParameters;   
    for(size_t i = 0; i < cnt; ++i)
    {
        QAction* act = tb->addAction(ToolBarStrings::pageNames[i]);
        settingPages->addWidget(pages[i]);
        connect(act, &QAction::triggered, settingPages, [this,i]() { settingPages->setCurrentIndex(i);});
    }

    
}

void
SettingsDialog::OnApply()
{
    for(auto it : pages)
    {
        it->Commit();
    }
}

void
SettingsDialog::OnOk()
{
    OnApply();
    done(0);
}

inline
std::array<ISettingPageWidget*, SettingsDialog::SettingPageId::cnt>
SettingsDialog::CreateSettingPagesArray()
{
    std::array<ISettingPageWidget*, SettingsDialog::SettingPageId::cnt> ret;
    ret[SettingPageId::common] = new CommonPage;
    return ret;
}