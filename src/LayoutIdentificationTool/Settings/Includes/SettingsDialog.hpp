#ifndef __SETTINGS_DIALOG_H__
#define __SETTINGS_DIALOG_H__

#include <QDialog>
#include <QWidget>
#include <QStackedWidget>

//#include "ISettingsPageWidget.hpp"

class SettingsDialog : public QDialog
{
    Q_OBJECT

    public:
    enum SettingPageId
    {
        nameAnalysis = 0,
        squareAnalysis,
        polyCntAnalysis,
        mutualControlAnalysis,
        bruteForcePolySearch,
        bitmapZonding,
        cnt
    };


    private:
    QStackedWidget* settingPages;

    
    public:

    explicit SettingsDialog(QWidget* Parent = nullptr) {}

    signals:

    void
    SettingsChanged();

    private:


    void
    Init();

    std::array<QWidget*, cnt>
    CreateSettingPagesArray();
};

#endif //!__SETTINGS_DIALOG_H__
