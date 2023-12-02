#include "MainWindow.hpp"

#include "Pages/IcLayoutPage/LcLayoutPage.hpp"

MainWindow::MainWindow(
   QWidget* Parent,
   Qt::WindowFlags Flags)
   : QMainWindow(Parent, Flags)
   , tabWidget(new QTabWidget)
{
   setMinimumSize(1200, 1000);
   setCentralWidget(tabWidget);
   tabWidget->addTab(new LcLayoutPage, "Топологии");
}