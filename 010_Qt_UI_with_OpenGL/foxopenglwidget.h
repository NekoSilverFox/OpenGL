#ifndef FOXOPENGLWIDGET_H
#define FOXOPENGLWIDGET_H

#include <QOpenGLWidget>  // 相当于GLFW
#include <QOpenGLFunctions_4_5_Core>  // 相当于 GLAD

class FoxOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    enum Shape
    {
        None,
        Rect,
        Circle,
        Triangle
    };

    explicit FoxOpenGLWidget(QWidget *parent = nullptr);
    void drawShape(Shape shape);
    void setWirefame(bool wirefame);
    ~FoxOpenGLWidget();


protected:
    /* 需要重载的 QOpenGLWidget 中的三个函数 */
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:

public slots:

private:
    Shape current_shape_;  // 记录当前绘制的图形
};

#endif // FOXOPENGLWIDGET_H
