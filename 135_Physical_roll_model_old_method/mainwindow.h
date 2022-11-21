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

    void on_actionDrawCube_triggered();

    void on_actionMoveCube_triggered();

    void on_actionWireframe_triggered();

    void on_actionLightColor_triggered();

    void on_actionRotate_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
