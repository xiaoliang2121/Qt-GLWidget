#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Move Block with Arrow Keys to see blending");
    w.show();

    return a.exec();
}
