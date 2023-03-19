#ifndef __SETTINGS_DIALOG_H__
#define __SETTINGS_DIALOG_H__

#include <QDialog>
#include <QWidget>
#include <QStackedWidget>
#include <QToolBar>

#include "SettingsPages.hpp"

//#include "ISettingsPageWidget.hpp"

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
        cnt
    };

    private:
    SettingPageId currentPage;
    QToolBar*       tb;
    QStackedWidget* settingPages;

    public:

    explicit
    SettingsDialog(
        QWidget* Parent = nullptr)
        : QDialog(Parent)
        , tb(new QToolBar)
        , settingPages(new QStackedWidget) 
        {
            QVBoxLayout* mainLayout = new QVBoxLayout(this);
            tb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            settingPages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            Init();
            mainLayout->addWidget(tb);
            mainLayout->addWidget(settingPages);
        }

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
