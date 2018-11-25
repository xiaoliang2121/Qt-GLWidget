#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget,&GLWidget::changeTitle,this,&MainWindow::onChangeWindowTitle);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChangeWindowTitle(QString title)
{
    setWindowTitle(title);
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
