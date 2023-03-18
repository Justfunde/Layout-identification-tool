#ifndef __SETTINGS_HANDLER_H__
#define __SETTINGS_HANDLER_H__

#include <QStringList>

namespace SettingsHandler
{
    namespace Common
    {
        QString
        GetCurrentLibPath();

        QStringList
        GetLibPathHistory();

        void
        SetCurrentLibPath(
            const QString& Path);

        void
        SetLibPathHistory(
            const QStringList PathHistory);
    }

    namespace BitmapZonding
    {
        
    }
}

#endif //!__SETTINGS_HANDLER_H__