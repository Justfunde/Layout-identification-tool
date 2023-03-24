#ifndef __SETTINGS_DIALOG_H__
#define __SETTINGS_DIALOG_H__

#include <QDialog>
#include <QWidget>
#include <QStackedWidget>
#include <QToolBar>

#include "SettingsPages.hpp"

class SettingsDialog : public QDialog
{
    Q_OBJECT

    public:
    enum SettingPageId
    {
        common = 0,
        nameAnalysis,
        squareAnalysis,
        polyCntAnalysis,
        mutualControlAnalysis,
        bruteForcePolySearch,
        bitmapZonding,
        cnt = nameAnalysis
    };

    private:
    SettingPageId currentPage;
    QToolBar*       tb;
    QStackedWidget* settingPages;

    std::array<ISettingPageWidget*, cnt> pages;

    public:

    explicit
    SettingsDialog(
        QWidget* Parent = nullptr);

    signals:

    void
    SettingsChanged();

    private slots:

    void
    OnApply();

    void
    OnOk();

    private:

    void
    Init();

    std::array<ISettingPageWidget*, cnt>
    CreateSettingPagesArray();
};

#endif //!__SETTINGS_DIALOG_H__
