#include "mainwindow.h"

#include <QWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.resize(600, 600);
    window.show();
    return a.exec();
}
