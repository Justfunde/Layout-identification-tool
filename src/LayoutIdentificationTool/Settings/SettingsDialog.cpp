#include "Headers/SettingsDialog.hpp"
#include "Headers/SettingsPages.hpp"

#include <QToolBar>

#include <array>
#include <string_view>
#include <QString>


namespace SettingParameters
{
    namespace ToolBarStrings
    {
        const QString pageNames[SettingsDialog::SettingPageId::cnt] = { 
            "Общее",
            "Алгоритм анализа имен ячеек",
            "Алгоритм сравнения площадей ячеек",
            "Алгоритм анализа числа полигонов и распределения точек в ячейках",
            "Алгоритм анализа взаимного расположения ячеек",
            "Алгоритм поиска полигонов методом грубой силы",
            "Алгоритм детального анализа" };
    }
}


void
SettingsDialog::Init()
{
    tb->setStyleSheet("QToolBar {border-bottom: 2px solid black; border-top: 2px solid black;}");

    const auto settingPagesArray = CreateSettingPagesArray();

    std::array<QAction*, cnt> actArr;

    using namespace SettingParameters;   
    for(size_t i = 0; i < 1 /*cnt*/; ++i)
    {
        QAction* act = tb->addAction(ToolBarStrings::pageNames[i]);
        settingPages->addWidget(settingPagesArray[i]);
        actArr[i] = act;
    }

    connect(actArr[SettingPageId::common], &QAction::triggered, settingPages, [this]() { settingPages->setCurrentIndex(SettingPageId::common);});
}

inline
std::array<QWidget*, SettingsDialog::SettingPageId::cnt>
SettingsDialog::CreateSettingPagesArray()
{
    std::array<QWidget*, SettingsDialog::SettingPageId::cnt> ret;
    ret[SettingPageId::common] = new CommonPage;
    return ret;
}