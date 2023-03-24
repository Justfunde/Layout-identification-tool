#ifndef __I_SETTINGS_PAGE_WIDGET_H__
#define __I_SETTINGS_PAGE_WIDGET_H__

#include <QGroupBox>

class ISettingPageWidget : public QGroupBox
{
    Q_OBJECT

    public:

    explicit ISettingPageWidget(QWidget* Parent = nullptr) : QGroupBox(Parent) {}


    signals:

    void SettingsChanged();


    void CompleteChanges();

    public slots:

    virtual
    void
    InitPage() = 0;

    virtual
    void 
    Commit() = 0;
};


#endif //! __I_SETTINGS_PAGE_WIDGET_H__