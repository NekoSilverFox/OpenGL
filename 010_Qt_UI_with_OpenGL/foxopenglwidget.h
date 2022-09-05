#ifndef FOXOPENGLWIDGET_H
#define FOXOPENGLWIDGET_H

#include <QOpenGLWidget>  // 相当于GLFW
#include <QOpenGLFunctions_4_5_Core>  // 相当于 GLAD

class FoxOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit FoxOpenGLWidget(QWidget *parent = nullptr);
    ~FoxOpenGLWidget();

protected:
    /* 需要重载的 QOpenGLWidget 中的三个函数 */
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:

public slots:
};

#endif // FOXOPENGLWIDGET_H
