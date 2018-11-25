#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this,&MainWindow::menuVal,ui->widget,&GLWidget::ProcessMenu);
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

void MainWindow::on_pushButton_2_clicked()
{
    float yRot = ui->widget->getyRot();

    yRot += 5.0f;

    if(yRot > 360.0f)
        yRot = 5.0f;

    ui->widget->setyRot(yRot);
}

void MainWindow::on_actionToggle_depth_test_triggered()
{
    emit menuVal(1);
}

void MainWindow::on_actionToggle_cull_backface_triggered()
{
    emit menuVal(2);
}

void MainWindow::on_actionSet_Fill_Mode_triggered()
{
    emit menuVal(3);
}

void MainWindow::on_actionSet_Line_Mode_triggered()
{
    emit menuVal(4);
}

void MainWindow::on_actionSet_Point_Mode_triggered()
{
    emit menuVal(5);
}
