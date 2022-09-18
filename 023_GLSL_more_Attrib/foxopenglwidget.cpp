#include <QDebug>
#include <QTime>
#include "foxopenglwidget.h"

// 顶点数据
float vertices[] = {
     0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   // 右上角 0
     0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  // 右下角 1
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, // 左下角 2
    -0.5f,  0.5f, 0.0f,    0.5f, 0.5f, 0.5f   // 左上角 3
};

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

// 创建 VAO 和 VBO 对象并且赋予 ID
unsigned int VBO, VAO;

// 创建 EBO 元素缓冲对象
unsigned int EBO;

// 顶点着色器的源代码，顶点着色器就是把 xyz 原封不动的送出去
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";


// 片段着色器的源代码，片段着色器就是给一个固定的颜色
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

FoxOpenGLWidget::FoxOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    this->current_shape_ = Shape::None;

    /* 每隔1ms取一次时间（发送一次信号） */
//    this->timer_.start(1);
//    connect(&this->timer_, SIGNAL(timeout()),
//            this, SLOT(changeColorWithTime()));

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


    // ===================== 顶点着色器 =====================
//    this->shader_program_.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);  // 通过字符串对象添加
    this->shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ShaderSource/source.vert");  // 通过资源文件

    // ===================== 片段着色器 =====================
//    this->shader_program_.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    this->shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ShaderSource/source.frag");

    // ===================== 链接着色器 =====================
    bool success = this->shader_program_.link();

    if (!success)
    {
        qDebug() << "ERROR: " << this->shader_program_.log();
    }


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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // 手动传入第几个属性
    glEnableVertexAttribArray(aPosLocation); // 开始 VAO 管理的第一个属性值

    this->shader_program_.bind();
    GLint aColorLocation = this->shader_program_.attributeLocation("aColor");
    glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aColorLocation);
#endif

#if 0
    /* 当我们在顶点着色器中没有写 layout 时，也可以在此处代码根据名字手动指定某个顶点属性的位置 */
    this->shader_program_.bind();
    GLint aPosLocation = 2;
    this->shader_program_.bindAttributeLocation("aPos", aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

#endif


    // ===================== EBO =====================
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // EBO/IBO 是储存顶点【索引】的


    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    /* 用线条填充，默认是 GL_FILL */
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

    /* 【重点】使用 QOpenGLShaderProgram 进行着色器绑定 */
    this->shader_program_.bind();

    /* 绘制三角形 */
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 代表6个点，因为一个矩形是2个三角形构成的，一个三角形有3个点
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);  // 直接到索引数组里去绘制，如果VAO没有绑定EBO的话

    // 通过 this->current_shape_ 确定当前需要绘制的图形
    switch (this->current_shape_)
    {
    case Shape::None:
        break;

    case Shape::Rect:
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        break;

    case Shape::Circle:
        break;

    case Shape::Triangle:
        break;

    default:
        break;

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
