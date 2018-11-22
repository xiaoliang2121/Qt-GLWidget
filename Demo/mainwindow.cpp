#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    float xRot = ui->widget->getxRot();

    xRot += 5.0f;

    if(xRot > 360.0f)
        xRot = 5.0f;

    ui->widget->setxRot(xRot);
}
