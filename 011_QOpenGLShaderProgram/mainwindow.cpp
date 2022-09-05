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

void MainWindow::on_actionDrawRect_triggered()
{
    ui->openGLWidget->drawShape(FoxOpenGLWidget::Shape::Rect);
}

void MainWindow::on_actionWireframe_triggered()
{
    ui->openGLWidget->setWirefame(ui->actionWireframe->isChecked());  // 如果按下就是线框模式
}

void MainWindow::on_actionClear_triggered()
{
    ui->openGLWidget->drawShape(FoxOpenGLWidget::Shape::None);
}
