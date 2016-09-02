#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Starlino");
    QCoreApplication::setApplicationName("SerialChart");
    MainWindow w;
    w.show();
    return a.exec();
}
