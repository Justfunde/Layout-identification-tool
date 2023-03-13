#include <QApplication>

#include "MainWindow.hpp"


int main(int Argc, char* Argv[])
{
    QApplication app(Argc, Argv);

    MainWindow w;

    w.show();
    return app.exec();
}