#include "axbopenglwidget.h"

AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）
}

void AXBOpenGLWidget::resizeGL(int w, int h)
{

}

void AXBOpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.32f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glClear(GL_COLOR_BUFFER_BIT);  // use方法
}
