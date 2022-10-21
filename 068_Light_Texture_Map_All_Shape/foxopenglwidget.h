#ifndef FOXOPENGLWIDGET_H
#define FOXOPENGLWIDGET_H


#include <QOpenGLWidget>  // 相当于GLFW
#include <QOpenGLFunctions_3_3_Core>  // 相当于 GLAD
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QTime>
#include <QOpenGLTexture>
#include "camera.hpp"
#include "sphere.hpp"
#include "cone.hpp"
#include "cube.hpp"
#include "light.h"



class FoxOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
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

    /* 移动图形 */
    void move3DShape(QVector3D step);


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
    void updateGL();

public:
    bool is_draw_sphere;
    bool is_move_sphere;

    bool is_draw_cone;
    bool is_move_cone;

    bool is_draw_cube;
    bool is_move_cube;

private:
    Shape current_shape_;  // 记录当前绘制的图形

    /* 球体 */
    Sphere _sphere;
    QOpenGLShaderProgram _sp_sphere;

    /* 椎体 */
    Cone _cone;
    QOpenGLShaderProgram _sp_cone;

    /* 立方体 */
    Cube _cube;
    QOpenGLShaderProgram _sp_cube;
    QOpenGLTexture* _texPoly;
    int _indexPoly;
    QOpenGLTexture* _texPolySpecular;
    int _indexTexPolySpecular;

    /* 时钟及计时器 */
    QTimer timer_;  // 【重点】 这里是 Timer - 计时器类
    QTime time_;

    /* 封装的摄像机类 */
    Camera camera_;

    /* 光源 */
    Light _light;
    QOpenGLShaderProgram _sp_light;
};

#endif // FOXOPENGLWIDGET_H
