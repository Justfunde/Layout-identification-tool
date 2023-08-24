#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:

   explicit 
   MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

   ~MainWindow() { }

   private:
   QTabWidget* tabWidget;
};

#endif //!__MAIN_WINDOW_H__