#ifndef FOXOPENGLWIDGET_H
#define FOXOPENGLWIDGET_H


#include <QOpenGLWidget>  // 相当于GLFW
#include <QOpenGLFunctions_4_5_Core>  // 相当于 GLAD
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QTime>
#include <QOpenGLTexture>
#include "camera.hpp"


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

    explicit FoxOpenGLWidget(QWidget* parent = nullptr);
    ~FoxOpenGLWidget();

    /* 设置绘制的图形 */
    void drawShape(Shape shape);

    /* 设置线框模式是否开启 */
    void setWirefame(bool wirefame);


protected:
    /* 需要重载的 QOpenGLWidget 中的三个函数 */
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    /* 处理键盘、鼠标事件 */
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);    // 鼠标移动
    void wheelEvent(QWheelEvent* event);        // 鼠标滚轮

signals:

public slots:
    void changeColorWithTime();
    void rotate();

private:
    Shape current_shape_;  // 记录当前绘制的图形
    QOpenGLShaderProgram shader_program_;  // 【重点】使用 Qt 提供的着色器对象

    /* 时钟及计时器 */
    QTimer timer_;  // 【重点】 这里是 Timer - 计时器类
    QTime time_;

    /* 纹理对象 */
    QOpenGLTexture* texture_wall_;  // 存储砖墙那张图片数据
    QOpenGLTexture* texture_nekosilverfox_;
    QOpenGLTexture* texture_nekosilverfox_bk_;

    /* 封装的摄像机类 */
    Camera camera_ = Camera(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), 50.0f, -90.0f, 0.0f);
};

#endif // FOXOPENGLWIDGET_H
