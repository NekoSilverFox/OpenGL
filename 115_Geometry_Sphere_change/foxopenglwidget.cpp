#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include "foxopenglwidget.h"
#include "sphere.hpp"


#define TIMEOUT 50  // 50 毫秒更新一次

const unsigned int NUM_VBO = 2;
const unsigned int NUM_VAO = 2;

/* 创建 VAO、VBO 对象并且赋予 ID */
unsigned int VBOs[NUM_VBO], VAOs[NUM_VAO];

unsigned long long gl_time = 0;


FoxOpenGLWidget::FoxOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    this->_sphere = Sphere(1.0f, 5.0f);
    this->_light = Light(1.0f, QVector3D(1.0f, 1.0f, 1.0f),
                               QVector3D(0.3f, 0.3f, 0.3f),
                               QVector3D(0.5f, 0.5f, 0.5f),
                               QVector3D(1.0f, 1.0f, 1.0f));

    is_draw_sphere = false;

    is_move_sphere = false;

    this->camera_ = Camera(QVector3D(-1.0f, 1.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), 50.0f, -90.0f, -20.0f);

    /* 暂时用于键盘点击事件 */
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    /* 计时器（时钟）每隔 TIMEOUT毫秒 取一次时间（发送一次信号） */
    this->timer_.start(TIMEOUT);
    connect(&this->timer_, SIGNAL(timeout()),
            this, SLOT(updateGL()));
}


FoxOpenGLWidget::~FoxOpenGLWidget()
{
    if (!isValid()) return;  // 如果 paintGL 没有执行，下面的代码不存在（着色器 VAO VBO之类的），所以避免出错。如果他们没有执行就直接 return

    makeCurrent();

    /* 对象的回收 */
    glDeleteVertexArrays(NUM_VAO, VAOs);
    glDeleteBuffers(NUM_VBO, VBOs);

    doneCurrent();
    update();
}


/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void FoxOpenGLWidget::initializeGL()
{
    qDebug() << "[INFO] initializeOpenGLFunctions(): " << initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）

    // ------------------------ VAO | VBO  | EBO------------------------
    // VAO 和 VBO 对象赋予 ID
    glGenVertexArrays(NUM_VAO, VAOs);
    glGenBuffers(NUM_VBO, VBOs);



    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ void FoxOpenGLWidget::initializeGL() @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\

/****************************************************** 球体 ******************************************************/
    // ------------------------ 着色器 ------------------------
    _sp_sphere.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/sphere.vert");
    _sp_sphere.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/ShaderSource/sphere.geom");
    _sp_sphere.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/sphere.frag");
    bool success = _sp_sphere.link();
    qDebug() << "[INFO] Sphere Shade Program _sp_sphere" << success;
    if (!success)
    {
        qDebug() << "[ERROR-Sphere] " << _sp_sphere.log();
    }

    _sp_sphere.bind();


    // ------------------------ VAO、VBO ------------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);


    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_sphere.vertices.size()*3, &_sphere.vertices[0], GL_STATIC_DRAW);

    _sp_sphere.bind();
    GLint aPosLocation = _sp_sphere.attributeLocation("aPos");
    glVertexAttribPointer(aPosLocation,     3,  GL_FLOAT,   GL_FALSE,   6 * sizeof(float),      (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    _sp_sphere.bind();
    GLint aNormalLocation = _sp_sphere.attributeLocation("aNormal");
    glVertexAttribPointer(aNormalLocation,  3,  GL_FLOAT,   GL_FALSE,   6 * sizeof(float),      (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(aNormalLocation);

    _sphere.mat_model.scale(0.7);
    _sphere.mat_model.translate(-2.5f, -0.5f, 0.0f);


    // ------------------------ 解绑 ------------------------
    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO



    /****************************************************** 光源 ******************************************************/
    // ------------------------ 着色器 ------------------------
    _sp_light.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/light.vert");
    _sp_light.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/light.frag");
    success = _sp_light.link();
    qDebug() << "[INFO] Sphere Shade Program _sp_light" << success;
    if (!success)
    {
         qDebug() << "[ERROR-Light] " << _sp_light.log();
    }

    _sp_light.bind();
    _sp_light.setUniformValue("light_color", _light.color_specular);

    // 绑定 VAO、VBO 对象
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_light.vertices.size(), &_light.vertices[0], GL_STATIC_DRAW);

    _sp_light.bind();
    aPosLocation = _sp_light.attributeLocation("aPos");
    glVertexAttribPointer(aPosLocation,     3,  GL_FLOAT,   GL_FALSE,       3 * sizeof(float),  (void*)0);
    glEnableVertexAttribArray(aPosLocation); // 开始 VAO 管理的第一个属性值

    _light.mat_model.translate(_light.postion);
    _light.mat_model.scale(0.2);

    _light.color_shininess = 128.0f;

    // ------------------------ 解绑 ------------------------
    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO

}


void FoxOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}




/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ void FoxOpenGLWidget::paintGL() @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void FoxOpenGLWidget::paintGL()
{
    /* 设置 OpenGLWidget 控件背景颜色为深青色，并且设置深度信息（Z-缓冲） */
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glEnable(GL_DEPTH_TEST);  // 深度信息，如果不设置立方体就像没有盖子
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // use方法

    QMatrix4x4 mat_view;  // 【重点】 view代表摄像机拍摄的物体，也就是全世界！！！
    mat_view = camera_.getViewMatrix();

    QMatrix4x4 mat_projection;  // 透视（焦距）一般设置一次就好了，之后不变。如果放在PaintGL() 里会导致每次重绘都调用，增加资源消耗
    mat_projection.perspective(camera_.zoom_fov, (float)width()/(float)height(), 0.1f, 100.0f);




    /****************************************************** 光源 ******************************************************/

    glBindVertexArray(VAOs[1]);

    _sp_light.bind();
    _sp_light.setUniformValue("mat_view", mat_view);
    _sp_light.setUniformValue("mat_projection", mat_projection);
    _sp_light.setUniformValue("mat_model", _light.mat_model);

    _sp_light.setUniformValue("light_color", _light.color_specular);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    /****************************************************** 球体 ******************************************************/
    if (is_draw_sphere)
    {
        glBindVertexArray(VAOs[0]);

        /* 【重点】使用 QOpenGLShaderProgram 进行着色器绑定 */
        _sp_sphere.bind();
        /* 摄像机矩阵 */
        _sp_sphere.setUniformValue("mat_view", mat_view);
        /* 透视 */
        _sp_sphere.setUniformValue("mat_projection", mat_projection);

        /* 材质颜色 */
        _sp_sphere.setUniformValue("material.ambient",    QVector3D(0.0215f,    0.1745f,    0.1215f));
        _sp_sphere.setUniformValue("material.diffuse",    QVector3D(0.07568f,   0.61424f,   0.07568f));
        _sp_sphere.setUniformValue("material.specular",   QVector3D(0.633f,     0.727811f,  0.633f));
        _sp_sphere.setUniformValue("material.shininess",  16.0f);

        _sp_sphere.setUniformValue("time",  (GLfloat)  sin(::gl_time / 10.0f));
        _sp_sphere.setUniformValue("del_h", (GLfloat)((sin(::gl_time / 20.0f) + 2.0f) * 0.5f));
        _sp_sphere.setUniformValue("del_w", (GLfloat)((cos(::gl_time / 20.0f) + 1.0f) * 0.5f));
//        _sp_sphere.setUniformValue("del_w", 1.0f);
        _sp_sphere.setUniformValue("step_angle", 2);
        qDebug() << "\n--------------------";
        qDebug() << "del_h" << (GLfloat)((sin(::gl_time / 20.0f) + 2.0f) * 0.5);
        qDebug() << "del_w" << (GLfloat)(abs(cos(::gl_time / 20.0f)));

        /* 光源颜色 */
        _sp_sphere.setUniformValue("light.ambient",    _light.color_ambient);
        _sp_sphere.setUniformValue("light.diffuse",    _light.color_diffuse);
        _sp_sphere.setUniformValue("light.specular",   _light.color_specular);
        _sp_sphere.setUniformValue("light.shininess",  _light.color_shininess);

        _sp_sphere.setUniformValue("light_pos", _light.postion);
        _sp_sphere.setUniformValue("view_pos", camera_.position);

        /* 模型操作 */
        _sp_sphere.setUniformValue("mat_model", _sphere.mat_model);

        glDrawArrays(GL_TRIANGLES, 0, _sphere.getNumTrianglesinSphere());
        glBindVertexArray(0);
    }


}


void FoxOpenGLWidget::setWirefame(bool wirefame)
{
    makeCurrent();

     /* 用线条填充，默认是 GL_FILL */
    if (true == wirefame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    doneCurrent();
    update();  // 【重点】注意使用 update() 进行重绘，也就是这条语句会重新调用 paintGL()
}


void FoxOpenGLWidget::move3DShape(QVector3D step)
{
    if (is_move_sphere)
    {
        _sphere.mat_model.translate(step);
    }
}


/* 处理键盘事件 */
void FoxOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = (float)TIMEOUT / (float)1000;
    float moveSpeed = 0.05;

    switch (event->key())
    {

    /* 键盘WASD移动摄像机 */
    case Qt::Key_W:     camera_.moveCamera(Camera_Movement::FORWARD,    cameraSpeed);  break;
    case Qt::Key_A:     camera_.moveCamera(Camera_Movement::LEFT,       cameraSpeed);  break;
    case Qt::Key_S:     camera_.moveCamera(Camera_Movement::BACKWARD,   cameraSpeed);  break;
    case Qt::Key_D:     camera_.moveCamera(Camera_Movement::RIGHT,      cameraSpeed);  break;
    case Qt::Key_Space: camera_.moveCamera(Camera_Movement::UP,         cameraSpeed);  break;
    case Qt::Key_Shift: camera_.moveCamera(Camera_Movement::DOWN,       cameraSpeed);  break;

    case Qt::Key_Up:    move3DShape(QVector3D( 0.0f,      moveSpeed, 0.0f));  break;
    case Qt::Key_Down:  move3DShape(QVector3D( 0.0f,     -moveSpeed, 0.0f));  break;
    case Qt::Key_Left:  move3DShape(QVector3D(-moveSpeed, 0.0f,      0.0f));  break;
    case Qt::Key_Right: move3DShape(QVector3D( moveSpeed, 0.0f,      0.0f));  break;

    default: break;
    }
}


/* 处理鼠标移动事件 */
void FoxOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{  
    /* 鼠标位置偏移量 */
    static QPoint delta_pos = QPoint(0.0f, 0.0f);
    static QPoint last_pos(event->pos());

    /* 鼠标没点击时的位置，当鼠标按键按下后停止记录 */
    if (!event->buttons())
    {
        last_pos = event->pos();
    }

    /* 鼠标按键点击后 */
    if (event->buttons() == Qt::LeftButton)
    {
        auto current_pos = event->pos();
        delta_pos = current_pos - last_pos;

        camera_.changeCameraFront(delta_pos.x(), delta_pos.y(), true);
        last_pos = current_pos;
        update();
    }
}


/* 处理鼠标滚轮事件 */
void FoxOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    camera_.changeCameraZoomFov(event->angleDelta().y()/120);  // 一步是 120

    update();
}


void FoxOpenGLWidget::updateGL()
{
    gl_time += 1;

//    _sphere.mat_model.rotate(0.5f, 0.0f, 1.0f, 0.4f);



    /* 旋转光源 */
    _light.postion.setX(cos(gl_time / 50.0) * 2.5);
    _light.postion.setY(0.5);
    _light.postion.setZ(sin(gl_time / 50.0) * 2.5);
    _light.mat_model.setToIdentity();
    _light.mat_model.translate(_light.postion);
    _light.mat_model.scale(0.2);



    /* 改变光源颜色光源 */
    if (true == is_change_light_color)
    {
        float color_x = sin(gl_time / 50.0 * 2.0f) + 0.45f;
        float color_y = sin(gl_time / 50.0 * 0.7f) + 0.4f;
        float color_z = sin(gl_time / 50.0 * 1.3f) + 0.2f;
        QVector3D new_color = QVector3D(color_x, color_y, color_z);

        _light.color_ambient = new_color * 0.2f;  // 乘以一个数来减少影响
        _light.color_diffuse = new_color * 0.5f;
        _light.color_specular = new_color;
    }
    else
    {
        _light.color_ambient  = QVector3D(0.4f, 0.4f, 0.4f);  // 乘以一个数来减少影响
        _light.color_diffuse  = QVector3D(0.5f, 0.5f, 0.5f);
        _light.color_specular = QVector3D(1.0f, 1.0f, 1.0f);
    }


    update();
}

