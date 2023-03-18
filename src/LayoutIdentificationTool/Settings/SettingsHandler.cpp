#include "Headers/SettingsHandler.hpp"

#include <QSettings>
#include <QThread>
#include <QSharedPointer>


#include <string_view>

constexpr std::string_view g_settingsPath = "Settings.ini";

namespace Groups
{
    namespace Names
    {
        const QString common = "Common";
    }

    namespace Common
    {
        const QString currentLibPath = "CurrLib";
        const QString historyPath    = "LibPathHistory";
    }
}

using SettingsSingletonPtr = QSharedPointer<QSettings>;

static
SettingsSingletonPtr
InSettingsInstance()
{
    static bool inited = false;
    static SettingsSingletonPtr instance(new QSettings(QString(g_settingsPath.data()), QSettings::Format::IniFormat));
    static QThread settingsThread;
    if(!inited)
    {
        instance->moveToThread(&settingsThread);
        inited = true;
    }
    return instance;
}


namespace SettingsHandler
{

QString
Common::GetCurrentLibPath()
{
    auto inst = InSettingsInstance();

    inst->beginGroup(Groups::Names::common);
    auto ret = inst->value(Groups::Common::currentLibPath).toString();
    inst->endGroup();
    return ret;
}


QStringList
Common::GetLibPathHistory()
{
    auto inst = InSettingsInstance();

    inst->beginGroup(Groups::Names::common);
    auto ret = inst->value(Groups::Common::historyPath).toStringList();
    inst->endGroup();
    return ret;
}


void
Common::SetCurrentLibPath(
    const QString& Path)
{
    auto inst = InSettingsInstance();
    QStringList ret;

    inst->beginGroup(Groups::Names::common);
    inst->setValue(Groups::Common::currentLibPath, Path);
    inst->endGroup();
}


void
Common::SetLibPathHistory(
    const QStringList PathHistory)
{
    auto inst = InSettingsInstance();
    QStringList ret;

    inst->beginGroup(Groups::Names::common);
    inst->setValue(Groups::Common::historyPath, PathHistory);
    inst->endGroup();
}

}
