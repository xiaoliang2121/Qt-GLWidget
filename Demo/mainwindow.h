#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionToggle_depth_test_triggered();

    void on_actionToggle_cull_backface_triggered();

    void on_actionSet_Fill_Mode_triggered();

    void on_actionSet_Line_Mode_triggered();

    void on_actionSet_Point_Mode_triggered();

signals:
    void menuVal(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
