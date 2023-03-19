#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "MainWindow.hpp"
#include "Settings/Headers/SettingsDialog.hpp"

MainWindow::MainWindow(
    QWidget* Parent)
    : QMainWindow(Parent)
    , toolBar(new QToolBar)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    setCentralWidget(new QWidget);

    mainLayout->addWidget(toolBar);
    
    QMenuBar* menuBar = new QMenuBar;
    menuBar->addMenu(CreateMenu());

    mainLayout->setMenuBar(menuBar);

    centralWidget()->setLayout(mainLayout);
    
}


void
MainWindow::InitToolBar()
{
    
}

QMenu*
MainWindow::CreateMenu()
{
    QMenu* menu = new QMenu("asd");

    QAction* settingsOpenAct = menu->addAction(tr("Настройки"));

    connect(settingsOpenAct, &QAction::triggered, this, &MainWindow::OpenSettings);

    return menu;
}

void
MainWindow::OpenSettings()
{
    SettingsDialog dial;
    dial.exec();    
}
