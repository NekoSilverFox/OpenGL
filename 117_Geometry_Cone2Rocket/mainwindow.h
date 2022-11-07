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
    void on_actionWireframe_triggered();

    void on_actionDrawSphere_triggered();

    void on_actionDrawCone_triggered();

    void on_actionMoveSphare_triggered();

    void on_actionMoveCone_triggered();

    void on_actionLightColor_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
