#include <QDebug>
#include <QTime>
#include <QKeyEvent>
#include "foxopenglwidget.h"
#include "sphere.h"


#define TIMEOUT 50  // 50 毫秒更新一次

/* 创建 VAO、VBO 对象并且赋予 ID */
unsigned int VBO, VAO;

/* 创建 EBO 元素缓冲对象 */
unsigned int EBO;

/* 透明度 */
float val_alpha = 0.5;

/* 鼠标位置偏移量 */
QPoint delta_pos;


FoxOpenGLWidget::FoxOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    this->current_shape_ = Shape::None;
    this->_sphere = Sphere(X_SPHERE_SEGMENTS, Y_SPHERE_SEGMENTS);

    this->camera_ = Camera(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), 50.0f, -90.0f, 0.0f);

    /* 暂时用于键盘点击事件 */
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    /* 计时器（时钟）每隔 TIMEOUT毫秒 取一次时间（发送一次信号） */
    this->timer_.start(TIMEOUT);
    connect(&this->timer_, SIGNAL(timeout()),
            this, SLOT(rotate()));

    this->time_.start();

}

FoxOpenGLWidget::~FoxOpenGLWidget()
{
    if (!isValid()) return;  // 如果 paintGL 没有执行，下面的代码不存在（着色器 VAO VBO之类的），所以避免出错。如果他们没有执行就直接 return

    makeCurrent();

    /* 对象的回收 */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    doneCurrent();
    update();
}


/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void FoxOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）


    // ===================== 着色器 =====================
    // 顶点着色器
    this->shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/source.vert");  // 通过资源文件

    // 片段着色器
    this->shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/source.frag");

    // 链接着色器
    bool success = this->shader_program_.link();
    if (!success)
    {
        qDebug() << "ERROR: " << this->shader_program_.log();
    }
    // =================================================


    // ===================== VAO | VBO =====================
    // VAO 和 VBO 对象赋予 ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO、VBO 对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /* 为当前绑定到 target 的缓冲区对象创建一个新的数据存储（在 GPU 上创建对应的存储区域，并将内存中的数据发送过去）
        如果 data 不是 NULL，则使用来自此指针的数据初始化数据存储
        void glBufferData(GLenum target,  // 需要在 GPU 上创建的目标
                                            GLsizeipter size,  // 创建的显存大小
                                            const GLvoid* data,  // 数据
                                            GLenum usage)  // 创建在 GPU 上的哪一片区域（显存上的每个区域的性能是不一样的）https://registry.khronos.org/OpenGL-Refpages/es3.0/
    */
    glBufferData(GL_ARRAY_BUFFER, _sphere.vertices.size()*sizeof(float), &_sphere.vertices[0], GL_STATIC_DRAW);

#if 1
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
    this->shader_program_.bind();  // 如果使用 QShaderProgram，那么最好在获取顶点属性位置前，先 bind()
    GLint aPosLocation = this->shader_program_.attributeLocation("aPos");  // 获取顶点着色器中顶点属性 aPos 的位置
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 手动传入第几个属性
    glEnableVertexAttribArray(aPosLocation); // 开始 VAO 管理的第一个属性值

//    this->shader_program_.bind();
//    GLint aTexelLocation = this->shader_program_.attributeLocation("aTexel");
//    glVertexAttribPointer(aTexelLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(aTexelLocation);

#endif

#if 0
    /* 当我们在顶点着色器中没有写 layout 时，也可以在此处代码根据名字手动指定某个顶点属性的位置 */
    this->shader_program_.bind();
    GLint aPosLocation = 2;
    this->shader_program_.bindAttributeLocation("aPos", aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

#endif
    // =================================================


    // ===================== EBO =====================
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_sphere.indices.size(), &_sphere.indices[0], GL_STATIC_DRAW);  // EBO/IBO 是储存顶点【索引】的
    // =================================================

#if 0
    // ===================== 纹理 =====================
    // 开启透明度
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->shader_program_.bind();
    this->shader_program_.setUniformValue("texture0", 0);  // 【重点】当涉及到多个纹理使，一定要为 uniform 设置纹理单元的编号
    this->texture_wall_ = new QOpenGLTexture(QImage(":/Pictures/wall.jpg").mirrored());  // 因为QOpenGL的y轴是反的（镜像），所以需要mirrored翻转一下

    this->shader_program_.bind();
    this->shader_program_.setUniformValue("texture1", 1);
    this->texture_nekosilverfox_ = new QOpenGLTexture(QImage(":/Pictures/nekosilverfox.png").mirrored());
    this->texture_nekosilverfox_->bind(1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    this->shader_program_.bind();
    this->shader_program_.setUniformValue("texture2", 2);
    this->texture_nekosilverfox_bk_ = new QOpenGLTexture(QImage(":/Pictures/nekosilverfox_bk.jpg").mirrored());
    // 纹理环绕方式
    this->texture_nekosilverfox_bk_->bind(2);  // 【重点】注意！再修改纹理之前要先绑定到对应的纹理单元上！
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);  // T轴纹理【环绕】方式
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // S轴纹理【环绕】方式
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // 缩小时轴纹理【过滤】方式
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 放大时纹理【过滤】方式
{  /* 【重点】如果在 Switch 里定义变量要放在花括号里，如果是颜色填充要先设置，再传入颜色*/ }
    // float bord_color[] = {1.0, 1.0, 0.0, 1.0};
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bord_color);
#endif
     this->shader_program_.bind();
     this->shader_program_.setUniformValue("val_alpha", val_alpha);
     // =================================================


    // ===================== 解绑 =====================
    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // =================================================
}


void FoxOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}


void FoxOpenGLWidget::paintGL()
{
    glViewport(0, 0, width(), height());

    /* 设置 OpenGLWidget 控件背景颜色为深青色，并且设置深度信息（Z-缓冲） */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glEnable(GL_DEPTH_TEST);  // 深度信息，如果不设置立方体就像没有盖子
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // use方法

    /* 重新绑定 VAO */
    glBindVertexArray(VAO);

    /* 【重点】使用 QOpenGLShaderProgram 进行着色器绑定 */
    this->shader_program_.bind();

    /* 绘制三角形 */
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 代表6个点，因为一个矩形是2个三角形构成的，一个三角形有3个点
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);  // 直接到索引数组里去绘制，如果VAO没有绑定EBO的话

    QMatrix4x4 mat_model; // QMatrix 默认生成的是一个单位矩阵（对角线上的元素为1）
    QMatrix4x4 mat_view;  // 【重点】 view代表摄像机拍摄的物体，也就是全世界！！！
    QMatrix4x4 mat_projection;  // 透视（焦距）一般设置一次就好了，之后不变。如果放在PaintGL() 里会导致每次重绘都调用，增加资源消耗

    mat_view = camera_.getViewMatrix();
    this->shader_program_.setUniformValue("mat_view", mat_view);  // 摄像机矩阵

    mat_projection.perspective(camera_.zoom_fov, (float)width()/(float)height(), 0.1f, 100.0f);  // 透视
    this->shader_program_.setUniformValue("mat_projection", mat_projection);


    // 通过 this->current_shape_ 确定当前需要绘制的图形
    float time = this->time_.elapsed() / 1000.0;  // 注意是 1000.0
    switch (this->current_shape_)
    {
    case Shape::None: break;

    case Shape::Rect:
        // ===================== 绑定纹理 =====================
//        this->texture_wall_->bind(0);  // 绑定纹理单元0的数据，并激活对应区域
//        this->texture_nekosilverfox_->bind(1);
//        this->texture_nekosilverfox_bk_->bind(2);

        mat_model.rotate(time * 10, 1.0f, 3.0f, 0.5f);  // 沿着转轴旋转图形
        this->shader_program_.setUniformValue("mat_model", mat_model);  // 图形矩阵
        glDrawElements(GL_TRIANGLES, _sphere.getNumTrianglesinSphere(), GL_UNSIGNED_INT, 0);
        break;

    case Shape::Circle: break;
    case Shape::Triangle: break;
    default: break;
    }   
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


void FoxOpenGLWidget::changeColorWithTime()
{
    if (this->current_shape_ == Shape::None) return;

    makeCurrent();

    int current_sec = QTime::currentTime().second();  // 取到秒
    float greenValue = sin(current_sec);
    this->shader_program_.setUniformValue("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

    doneCurrent();
    update();
}


/* 处理键盘事件 */
void FoxOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = (float)TIMEOUT / (float)1000;

    switch (event->key()) {

    /* 键盘上下键改变透明度 */
    case Qt::Key_Up: val_alpha += 0.1; break;
    case Qt::Key_Down: val_alpha -= 0.1; break;

    /* 键盘WASD移动摄像机 */
    case Qt::Key_W: camera_.moveCamera(Camera_Movement::FORWARD, cameraSpeed);  break;
    case Qt::Key_A: camera_.moveCamera(Camera_Movement::LEFT, cameraSpeed);     break;
    case Qt::Key_S: camera_.moveCamera(Camera_Movement::BACKWARD, cameraSpeed); break;
    case Qt::Key_D: camera_.moveCamera(Camera_Movement::RIGHT, cameraSpeed);    break;

    default: break;
    }

    if (val_alpha > 1.0) val_alpha = 1.0;
    if (val_alpha < 0.0) val_alpha = 0.0;

    makeCurrent();
    this->shader_program_.bind();
    this->shader_program_.setUniformValue("val_alpha", val_alpha);
    qDebug() << "[INFO] val_alpha=" << val_alpha;
    doneCurrent();
    update();

}


/* 处理鼠标移动事件 */
void FoxOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint last_pos(width()/2, height()/2);
    auto current_pos = event->pos();
    delta_pos = current_pos - last_pos;
    last_pos = current_pos;

    camera_.changeCameraFront(delta_pos.x(), delta_pos.y(), true);

    update();
}


/* 处理鼠标滚轮事件 */
void FoxOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    camera_.changeCameraZoomFov(event->angleDelta().y()/120);  // 一步是 120

    update();
}

void FoxOpenGLWidget::rotate()
{
    update();
}
