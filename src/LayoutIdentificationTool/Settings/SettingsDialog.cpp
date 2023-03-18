#include "Headers/SettingsDialog.hpp"

#include <QToolBar>

#include <array>
#include <string_view>
#include <QString>


namespace SettingParameters
{
    namespace ToolBarStrings
    {
        const QString pageNames[SettingsDialog::SettingPageId::cnt] = { 
            "Общее"
            "Алгоритм анализа имен ячеек",
            "Алгоритм сравнения площадей ячеек",
            "Алгоритм анализа числа полигонов и распределения точек в ячейках",
            "Алгоритм анализа взаимного расположения ячеек",
            "Алгоритм поиска полигонов методом грубой силы",
            "Алгоритм детального анализа" };
    }
}

/*SettingsDialog::SettingsDialog(
    QWidget* Parent)
    : QDialog(Parent)
{
    setWindowTitle("Настройки");

    Init();

}*/

void
SettingsDialog::Init()
{
    QToolBar* settingsSelectorToolBar = new QToolBar;
    settingsSelectorToolBar->setStyleSheet("QToolBar {border-bottom: 2px solid black; border-top: 2px solid black;}");

    const auto settingPagesArray = CreateSettingPagesArray();

    using namespace SettingParameters;   
    for(size_t i = 0; i < cnt; ++i)
    {
        QAction* act = settingsSelectorToolBar->addAction(ToolBarStrings::pageNames[i]);
    }

}

std::array<QWidget*, SettingsDialog::SettingPageId::cnt>
SettingsDialog::CreateSettingPagesArray()
{
    std::array<QWidget*, SettingsDialog::SettingPageId::cnt> ret;
    return ret;
}