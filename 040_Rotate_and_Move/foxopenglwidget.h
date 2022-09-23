#ifndef FOXOPENGLWIDGET_H
#define FOXOPENGLWIDGET_H

#include <QOpenGLWidget>  // 相当于GLFW
#include <QOpenGLFunctions_4_5_Core>  // 相当于 GLAD
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLTexture>

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
    ~FoxOpenGLWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wirefame);


protected:
    /* 需要重载的 QOpenGLWidget 中的三个函数 */
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    /* 处理键盘事件 */
    void keyPressEvent(QKeyEvent* event);

signals:

public slots:
    void changeColorWithTime();

private:
    Shape current_shape_;  // 记录当前绘制的图形
    QOpenGLShaderProgram shader_program_;  // 【重点】使用 Qt 提供的对象进行编译和链接
    QTimer timer_;  // 【重点】 这里是 Timer - 计时器类
    QOpenGLTexture* texture_wall_;  // 存储砖墙那张图片数据
    QOpenGLTexture* texture_nekosilverfox_;
    QOpenGLTexture* texture_nekosilverfox_bk_;
};

#endif // FOXOPENGLWIDGET_H
