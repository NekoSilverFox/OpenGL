#include <QDebug>
#include "foxopenglwidget.h"

// 顶点数据
float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角 0
    0.5f, -0.5f, 0.0f,  // 右下角 1
    -0.5f, -0.5f, 0.0f, // 左下角 2
    -0.5f, 0.5f, 0.0f   // 左上角 3
};


// 创建 VAO 和 VBO 对象并且赋予 ID
unsigned int VBO, VAO;

FoxOpenGLWidget::FoxOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

FoxOpenGLWidget::~FoxOpenGLWidget()
{
    makeCurrent();

    /* 对象的回收 */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    doneCurrent();
    update();
}


/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void FoxOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）
    glEnable(GL_PROGRAM_POINT_SIZE);


    // ===================== VAO | VBO =====================
    // VAO 和 VBO 对象赋予 ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO、VBO 对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 第一个属性，所以不需要偏移
    glEnableVertexAttribArray(0);

    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // ===================== 着色器 =====================
    this->shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/source.vert");
    this->shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/source.frag");
    bool success = this->shader_program_.link();
    if (!success) qDebug() << "ERROR: " << this->shader_program_.log();

}

void FoxOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void FoxOpenGLWidget::paintGL()
{
    /* 设置 OpenGLWidget 控件背景颜色为深青色 */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glClear(GL_COLOR_BUFFER_BIT);  // use方法

    /* 重新绑定 VAO */
    glBindVertexArray(VAO);
    this->shader_program_.bind();
    glDrawArrays(GL_POINTS, 0, 4);
}

void FoxOpenGLWidget::drawShape(FoxOpenGLWidget::Shape shape)
{
    this->current_shape_ = shape;
    update();  // 【重点】注意使用 update() 进行重绘，也就是这条语句会重新调用 paintGL()
}

void FoxOpenGLWidget::setWirefame(bool wirefame)
{
    makeCurrent();

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







