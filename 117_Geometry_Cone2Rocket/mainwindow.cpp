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


void MainWindow::on_actionDrawSphere_triggered()
{
    ui->openGLWidget->is_draw_sphere = ui->actionDrawSphere->isChecked();
    ui->openGLWidget->updateGL();
}

void MainWindow::on_actionDrawCone_triggered()
{
    ui->openGLWidget->is_draw_cone = ui->actionDrawCone->isChecked();
    ui->openGLWidget->updateGL();
}

void MainWindow::on_actionMoveSphare_triggered()
{
    ui->openGLWidget->is_move_sphere = ui->actionMoveSphare->isChecked();
}

void MainWindow::on_actionMoveCone_triggered()
{
    ui->openGLWidget->is_move_cone = ui->actionMoveCone->isChecked();
}


void MainWindow::on_actionLightColor_triggered()
{
    ui->openGLWidget->is_change_light_color = ui->actionLightColor->isChecked();
}


void MainWindow::on_action2Booster_triggered()
{
    ui->openGLWidget->is_draw_booster = ui->action2Booster->isChecked();
}


void MainWindow::on_action2Pyramid_triggered()
{
    ui->openGLWidget->is_2_pyramid = ui->action2Pyramid->isChecked();
}


void MainWindow::on_action2House_triggered()
{
    ui->openGLWidget->is_2_house = ui->action2House->isChecked();
}


void MainWindow::on_action2Pencil_triggered()
{
    ui->openGLWidget->is_2_pencil = ui->action2Pencil->isChecked();
}


void MainWindow::on_actionLaunch_triggered()
{
    ui->openGLWidget->is_launch = ui->actionLaunch->isChecked();
}

