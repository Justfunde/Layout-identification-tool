#ifndef __SETTINGS_PAGES_H__
#define __SETTINGS_PAGES_H__

#include "ISettingsPageWidget.hpp"
#include "Common/FileBrowser/FileBrowser.hpp"


#include <QVBoxLayout>
#include <QHBoxLayout>


class CommonPage : public ISettingPageWidget
{
    Q_OBJECT

    private:
    FileBrowser* fileBrowser;


    public:

    explicit
    CommonPage(
        QWidget* Parent = nullptr);

    public slots:

    virtual
    void
    InitPage() override final;

    virtual
    void 
    Commit() override final;

    private:

};


class NameAnalysisPage : public ISettingPageWidget
{
    Q_OBJECT

    public:

    explicit
    NameAnalysisPage(
        QWidget* Parent = nullptr);

    
    public slots:

    virtual
    void
    InitPage() override final;

    private:

};

class SquareAnalysisPage : public ISettingPageWidget
{
    Q_OBJECT

    public:

    explicit
    SquareAnalysisPage(
        QWidget* Parent = nullptr);

    private:

    public slots:

    virtual
    void
    InitPage() override final;

    private:

};

class PolyCntAnalysisPage : public ISettingPageWidget
{
    Q_OBJECT

    public:

    explicit
    PolyCntAnalysisPage(
        QWidget* Parent = nullptr);

    private:

    public slots:

    virtual
    void
    InitPage() override final;

    private:

};

class MutualControlAnalysisPage : public ISettingPageWidget
{
    Q_OBJECT

    public:

    explicit
    MutualControlAnalysisPage(
        QWidget* Parent = nullptr);

    private:

    public slots:

    virtual
    void
    InitPage() override final;

    private:

};

class BruteForcePolyAnalisysPage : public ISettingPageWidget
{
    Q_OBJECT

    public:

    explicit
    BruteForcePolyAnalisysPage(
        QWidget* Parent = nullptr);

    private:

    public slots:

    virtual
    void
    InitPage() override final;

    private:

};

class BitmapZondingPage : public ISettingPageWidget
{
    Q_OBJECT

    public:

    explicit
    BitmapZondingPage(
        QWidget* Parent = nullptr);

    private:

    public slots:

    virtual
    void
    InitPage() override final;

    private:

};


#endif //!__SETTINGS_PAGES_H__