#ifndef __I_SETTINGS_PAGE_WIDGET_H__
#define __I_SETTINGS_PAGE_WIDGET_H__

#include <QWidget>

class ISettingPageWidget : public QWidget
{
    Q_OBJECT

    public:

    explicit ISettingPageWidget(QWidget* Parent = nullptr) : QWidget(Parent) {}


    signals:

    void SettingsChanged();

    void Commit();

    void CompleteChanges();

    public slots:

    virtual
    void
    InitPage() = 0;
};


#endif //! __I_SETTINGS_PAGE_WIDGET_H__