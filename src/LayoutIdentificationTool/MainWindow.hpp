#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QToolBar>

class MainWindow: public QMainWindow
{
    Q_OBJECT
    private:
    QToolBar* toolBar;

    public:

    explicit MainWindow(QWidget* Parent = nullptr);

    private:

    void
    InitToolBar();

    QMenu*
    CreateMenu();

    private slots:

    void
    OpenSettings();

    
};


#endif //!__MAIN_WINDOW_H__