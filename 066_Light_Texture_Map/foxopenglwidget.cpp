#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include "foxopenglwidget.h"
#include "sphere.hpp"


#define TIMEOUT 50  // 50 毫秒更新一次

const unsigned int NUM_VBO = 4;
const unsigned int NUM_VAO = 4;
const unsigned int NUM_EBO = 2;

/* 创建 VAO、VBO 对象并且赋予 ID */
unsigned int VBOs[NUM_VBO], VAOs[NUM_VAO];

/* 创建 EBO 元素缓冲对象 */
unsigned int EBOs[NUM_EBO];

/* 透明度 */
float val_alpha = 0.5;

unsigned long long gl_time = 0;


FoxOpenGLWidget::FoxOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    this->_sphere = Sphere(X_SPHERE_SEGMENTS, Y_SPHERE_SEGMENTS);
    this->_cone = Cone(R, HEIGHT, 10.0);
    this->_cube = Cube(LENGTH, COLOR_CUBE);
    this->_light = Light(1.0f, QVector3D(1.0f, 1.0f, 1.0f));

    is_draw_sphere = false;
    is_draw_cone = false;
    is_draw_cube = false;

    is_move_sphere = false;
    is_move_cone = false;
    is_move_cube = false;

    this->camera_ = Camera(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), 50.0f, -90.0f, 0.0f);

    /* 暂时用于键盘点击事件 */
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    /* 计时器（时钟）每隔 TIMEOUT毫秒 取一次时间（发送一次信号） */
    this->timer_.start(TIMEOUT);
    connect(&this->timer_, SIGNAL(timeout()),
            this, SLOT(updateGL()));

//    this->time_.start();
}


FoxOpenGLWidget::~FoxOpenGLWidget()
{
    if (!isValid()) return;  // 如果 paintGL 没有执行，下面的代码不存在（着色器 VAO VBO之类的），所以避免出错。如果他们没有执行就直接 return

    makeCurrent();

    /* 对象的回收 */
    glDeleteVertexArrays(NUM_VAO, VAOs);
    glDeleteBuffers(NUM_VBO, VBOs);
    glDeleteBuffers(NUM_EBO, EBOs);

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
    glGenBuffers(NUM_EBO, EBOs);  // 立方体没有使用 EBO








/****************************************************** 球体 ******************************************************/
    // ------------------------ 着色器 ------------------------
    _sp_sphere.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/sphere.vert");
    _sp_sphere.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/sphere.frag");
    bool success = _sp_sphere.link();
    qDebug() << "[INFO] Sphere Shade Program _sp_sphere" << success;
    if (!success)
    {
        qDebug() << "[ERROR-Sphere] " << _sp_sphere.log();
    }

    _sp_sphere.bind();
    _sp_sphere.setUniformValue("val_alpha", val_alpha);


    // ------------------------ VAO、VBO ------------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    /* 为当前绑定到 target 的缓冲区对象创建一个新的数据存储（在 GPU 上创建对应的存储区域，并将内存中的数据发送过去）
        如果 data 不是 NULL，则使用来自此指针的数据初始化数据存储
        void glBufferData(GLenum target,  // 需要在 GPU 上创建的目标
                                            GLsizeipter size,  // 创建的显存大小
                                            const GLvoid* data,  // 数据
                                            GLenum usage)  // 创建在 GPU 上的哪一片区域（显存上的每个区域的性能是不一样的）https://registry.khronos.org/OpenGL-Refpages/es3.0/
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_sphere.vertices.size(), &_sphere.vertices[0], GL_STATIC_DRAW);

    /* 告知显卡如何解析缓冲区里面的属性值
        void glVertexAttribPointer(
                                    GLuint index,  // VAO 中的第几个属性（VAO 属性的索引）
                                    GLint size,  // VAO 中的第几个属性中对应的位置放几份数据
                                    GLEnum type,  // 存放数据的数据类型
                                    GLboolean normalized,  // 是否标准化
                                    GLsizei stride,  // 步长
                                    const void* offset  // 偏移量
        )
    */
    _sp_sphere.bind();  // 如果使用 QShaderProgram，那么最好在获取顶点属性位置前，先 bind()
    GLint aPosLocation = _sp_sphere.attributeLocation("aPos");  // 获取顶点着色器中顶点属性 aPos 的位置
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 手动传入第几个属性
    glEnableVertexAttribArray(aPosLocation); // 开始 VAO 管理的第一个属性值


    // ------------------------ EBO ------------------------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_sphere.indices.size(), &_sphere.indices[0], GL_STATIC_DRAW);  // EBO/IBO 是储存顶点【索引】的

    _sphere.mat_model.translate(-2.0f, 0.0f, 0.0f);
    _sphere.mat_model.scale(0.5);

    // ------------------------ 解绑 ------------------------
    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);










    /****************************************************** 锥体 ******************************************************/
    // ------------------------ 着色器 ------------------------
    _sp_cone.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/cone.vert");
    _sp_cone.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/cone.frag");
    success = _sp_cone.link();
    qDebug() << "[INFO] Sphere Shade Program _sp_cone" << success;
    if (!success)
    {
        qDebug() << "[ERROR-Cone] " << _sp_cone.log();
    }

    _sp_cone.bind();
    _sp_cone.setUniformValue("val_alpha", val_alpha);


    // ------------------------ VAO、VBO ------------------------
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_cone.vertices.size(), &_cone.vertices[0], GL_STATIC_DRAW);

    _sp_cone.bind();  // 如果使用 QShaderProgram，那么最好在获取顶点属性位置前，先 bind()
    aPosLocation = _sp_cone.attributeLocation("aPos");  // 获取顶点着色器中顶点属性 aPos 的位置
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 手动传入第几个属性
    glEnableVertexAttribArray(aPosLocation); // 开始 VAO 管理的第一个属性值


    // ------------------------ EBO ------------------------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_cone.indices.size(), &_cone.indices[0], GL_STATIC_DRAW);  // EBO/IBO 是储存顶点【索引】的


    _cone.mat_model.translate(0.0f, 0.0f, 0.0f);

    // ------------------------ 解绑 ------------------------
    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);








    /****************************************************** 立方体 ******************************************************/
    // ------------------------ 着色器 ------------------------
    _sp_cube.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/cube.vert");
    _sp_cube.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/cube.frag");
    success = _sp_cube.link();
    qDebug() << "[INFO] Sphere Shade Program _sp_cube" << success;
    if (!success)
    {
         qDebug() << "[ERROR-Cube] " << _sp_cube.log();
    }


    // 绑定 VAO、VBO 对象
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_cube.vertices.size(), &_cube.vertices[0], GL_STATIC_DRAW);

    _sp_cube.bind();
    aPosLocation = _sp_cube.attributeLocation("aPos");
    glVertexAttribPointer(aPosLocation,     3,  GL_FLOAT,   GL_FALSE,   8 * sizeof(float),  (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    _sp_cube.bind();
    int aNormalLocation = _sp_cube.attributeLocation("aNormal");
    glVertexAttribPointer(aNormalLocation,  3,  GL_FLOAT,   GL_FALSE,   8 * sizeof(float),  (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(aNormalLocation);

    _sp_cube.bind();
    int aTexColorLocation = _sp_cube.attributeLocation("aTexColor");
    glVertexAttribPointer(aTexColorLocation,2,  GL_FLOAT,   GL_FALSE,   8 * sizeof(float),  (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(aTexColorLocation);

    // ------------------------ 纹理 ------------------------
    _texWoodBox = new QOpenGLTexture(QImage(":/pic/Picture/wood_box.png").mirrored());
    _indexTexWoodBoox = 0;  // 设置为第 0 个纹理单元
    _sp_cube.setUniformValue("material.diffuse", _indexTexWoodBoox);  // 【复习 | 重点】一个着色器内可以绑定多个纹理，只需要不同纹理对应不同的索引即可

    _texWoodBoxSpecular = new QOpenGLTexture(QImage(":/pic/Picture/wood_box_specular_map.png").mirrored());
    _indexTexWoodBoxSpecular = 1;
    _sp_cube.setUniformValue("material.specular", _indexTexWoodBoxSpecular);  // 为他绑定第二个纹理单元

    _cube.mat_model.translate(0.0f, -0.5f, 0.0f);

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
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_light.vertices.size(), &_light.vertices[0], GL_STATIC_DRAW);

    _sp_light.bind();
    aPosLocation = _sp_light.attributeLocation("aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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


void FoxOpenGLWidget::paintGL()
{
//    glViewport(0, 0, width(), height());

    /* 设置 OpenGLWidget 控件背景颜色为深青色，并且设置深度信息（Z-缓冲） */
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glEnable(GL_DEPTH_TEST);  // 深度信息，如果不设置立方体就像没有盖子
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // use方法

    QMatrix4x4 mat_view;  // 【重点】 view代表摄像机拍摄的物体，也就是全世界！！！
    mat_view = camera_.getViewMatrix();

    QMatrix4x4 mat_projection;  // 透视（焦距）一般设置一次就好了，之后不变。如果放在PaintGL() 里会导致每次重绘都调用，增加资源消耗
    mat_projection.perspective(camera_.zoom_fov, (float)width()/(float)height(), 0.1f, 100.0f);


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

        /* 模型操作 */
        _sp_sphere.setUniformValue("mat_model", _sphere.mat_model);

        glDrawElements(GL_TRIANGLES, _sphere.getNumTrianglesinSphere(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    /****************************************************** 锥体 ******************************************************/
    if (is_draw_cone)
    {
        glBindVertexArray(VAOs[1]);

        _sp_cone.bind();
        _sp_cone.setUniformValue("mat_view", mat_view);
        _sp_cone.setUniformValue("mat_projection", mat_projection);
        _sp_cone.setUniformValue("mat_model", _cone.mat_model);

        glDrawElements(GL_TRIANGLES, _cone.getNumTrianglesinSphere(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    /****************************************************** 立方体 ******************************************************/
    if (is_draw_cube)
    {
        glBindVertexArray(VAOs[2]);

        _texWoodBox->bind(_indexTexWoodBoox);  // 绑定纹理
        _texWoodBoxSpecular->bind(_indexTexWoodBoxSpecular);

        _sp_cube.bind();
        _sp_cube.setUniformValue("mat_view", mat_view);
        _sp_cube.setUniformValue("mat_projection", mat_projection);
        _sp_cube.setUniformValue("mat_model", _cube.mat_model);

        /* 材质颜色 */
//        _sp_cube.setUniformValue("material.ambient",    QVector3D(1.0f, 0.5f, 0.31f));
//        _sp_cube.setUniformValue("material.diffuse",    QVector3D(1.0f, 0.5f, 0.31f));
//        _sp_cube.setUniformValue("material.specular",   QVector3D(0.5f, 0.5f, 0.5f));
        _sp_cube.setUniformValue("material.shininess",  128.0f);

        /* 光源颜色 */
        _sp_cube.setUniformValue("light.ambient",    _light.color_ambient);
        _sp_cube.setUniformValue("light.diffuse",    _light.color_diffuse);
        _sp_cube.setUniformValue("light.specular",   _light.color_specular);
        _sp_cube.setUniformValue("light.shininess",  _light.color_shininess);

        _sp_cube.setUniformValue("light_pos", _light.postion);
        _sp_cube.setUniformValue("view_pos", camera_.position);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }


    /****************************************************** 光源 ******************************************************/

        glBindVertexArray(VAOs[3]);

        _sp_light.bind();
        _sp_light.setUniformValue("mat_view", mat_view);
        _sp_light.setUniformValue("mat_projection", mat_projection);
        _sp_light.setUniformValue("mat_model", _light.mat_model);

        _sp_light.setUniformValue("light_color", _light.color_specular);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


}


void FoxOpenGLWidget::drawShape(FoxOpenGLWidget::Shape shape)
{
    this->current_shape_ = shape;
    update();  // 【重点】注意使用 update() 进行重绘，也就是这条语句会重新调用 paintGL()
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

    if (is_move_cone)
    {
        _cone.mat_model.translate(step);
    }

    if (is_move_cube)
    {
        _cube.mat_model.translate(step);
    }
}


/* 处理键盘事件 */
void FoxOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = (float)TIMEOUT / (float)1000;
    float moveSpeed = 0.05;

    switch (event->key())
    {

    /* 键盘 +/-键改变透明度 */
    case Qt::Key_Minus: val_alpha += 0.1; break;
    case Qt::Key_Plus: val_alpha -= 0.1; break;

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

    if (val_alpha > 1.0) val_alpha = 1.0;
    if (val_alpha < 0.0) val_alpha = 0.0;

    makeCurrent();
    _sp_sphere.bind();
    _sp_sphere.setUniformValue("val_alpha", val_alpha);

    qDebug() << "[INFO] val_alpha=" << val_alpha;
    doneCurrent();
    update();

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


    _cone.mat_model.rotate(-0.5f, 0.0f, 1.0f, 0.0f);
    _cube.mat_model.rotate( 0.7f, 0.0f, 1.0f, 0.1f);
    _sphere.mat_model.rotate(0.5f, 0.0f, 1.0f, 0.4f);


    /* 旋转光源 */
    _light.postion.setX(cos(gl_time / 50.0) * 2.5);
    _light.postion.setZ(sin(gl_time / 50.0) * 2.5);
    _light.mat_model.setToIdentity();
    _light.mat_model.translate(_light.postion);
    _light.mat_model.scale(0.2);
//    _light.mat_model.rotate(1.5f, 0.0f, 1.0f, 0.0f);


#if 0
    /* 改变光源颜色光源 */
    float color_x = sin(gl_time / 50.0 * 2.0f);
    float color_y = sin(gl_time / 50.0 * 0.7f);
    float color_z = sin(gl_time / 50.0 * 1.3f);
    QVector3D new_color = QVector3D(color_x, color_y, color_z);

    _light.color_ambient = new_color * 0.2f;  // 乘以一个数来减少影响
    _light.color_diffuse = new_color * 0.5f;
    _light.color_specular = new_color;
#endif
    update();
}

