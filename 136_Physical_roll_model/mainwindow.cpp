#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);  // 将 OpenGL 控件充满整个窗口
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionWireframe_triggered()
{
    ui->openGLWidget->setWirefame(ui->actionWireframe->isChecked());  // 如果按下就是线框模式
}

void MainWindow::on_actionDrawCube_triggered()
{
    ui->openGLWidget->is_draw_cube = ui->actionDrawCube->isChecked();
    ui->openGLWidget->updateGL();
}

void MainWindow::on_actionMoveCube_triggered()
{
    ui->openGLWidget->is_move_cube = ui->actionMoveCube->isChecked();
}

void MainWindow::on_actionLightColor_triggered()
{
    ui->openGLWidget->is_change_light_color = ui->actionLightColor->isChecked();
}


void MainWindow::on_actionRotate_triggered()
{
    ui->openGLWidget->is_rotate = ui->actionRotate->isChecked();
}

