//#include <GL/freeglut.h>
//#include <GLUT/glut.h>
#include "axbopenglwidget.h"
#include "QDebug"

// 顶点数据
float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};

// 贝塞尔曲线控制点
//GLfloat control_points[4][3] = {
//    {-0.4f, 0.1f, 0.0f},
//    {-0.1f, 0.1f, 0.0f},
//    {-0.1f, 0.4f, 0.0f},
//    {-0.4f, 0.4f, 0.0f}
//};
GLfloat control_points[4][4][3] = {
    {
        { -0.3, -0.3,  2.0 },
        { -0.2, -0.3,  2.0 },
        {  0.2, -0.3, -1.0 },
        {  0.3, -0.3,  2.0 }
    },

    {
        { -0.3, -0.2,  1.0 },
        { -0.2,  0.3,  2.0 },
        {  0.2,  0.2,  1.0 },
        {  0.3, -0.2, -1.0 }
    },

    {
        { -0.3,  0.2, 2.0 },
        { -0.2,  0.2, 1.0 },
        {  0.2,  0.2, 3.0 },
        {  0.3, -0.3, 0.3 }
    },

    {
        { -0.3, 0.3, -2.0 },
        { -0.2, 0.3, -2.0 },
        {  0.2, 0.2,  1.0 },
        {  0.3, 0.3, -1.0 }
    }
};


// 创建 VAO 和 VBO 对象并且赋予 ID
unsigned int VBO, VAO;

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

// 着色器程序（链接顶点和片段着色器之后生成的）
unsigned int shaderProgram;


AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void AXBOpenGLWidget::initializeGL()
{
    qDebug() << initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）


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


    // 解绑 VAO 和 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


//    glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 4, &control_points[0][0]);
//    glEnable(GL_MAP1_VERTEX_3);


    // ===================== 顶点着色器 =====================
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);  // 创建顶点着色器（框架 | 对象）并给予编号
    /* 绑定至着色器原码
        void glShaderSource(
                             GLuint shader,  着色器框架
                             GLsize count,  着色器字符串的数量
                             const CLchar** string,  着色器原码字符串
                             const GLint* length  着色器原码的长度，如果是单个字符串可以填 NULL（代表原码字符串以 NULL 结尾）
        )

    */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  // 绑定至着色器原码
    glCompileShader(vertexShader);  // 编译着色器

    /* 因为可能出错，所以进行错误检查，也就是判断时候成功编译 */
    int success;  // 是否成功的标志
    char infolog[512];  // 错误日志（信息）
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
      qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog;
    }


    // ===================== 片段着色器 =====================
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog;
    }


    // ===================== 链接着色器 =====================
    /* 链接顶点着色器和片段着色器，并生成最后的着色器[程序] */
    shaderProgram = glCreateProgram();  // 【注意】是 `glCreateProgram`
    glAttachShader(shaderProgram, vertexShader);  // 加入顶点着色器
    glAttachShader(shaderProgram, fragmentShader);  // 加入片段着色器
    glLinkProgram(shaderProgram);  // 链接

    /* 因为可能出错，所以进行错误检查，也就是判断时候成功链接 */
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(shaderProgram, 512, NULL, infolog);
      qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog;
    }

    /* 删除已经不需要的编译的结果 */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void AXBOpenGLWidget::resizeGL(int w, int h) {}

void AXBOpenGLWidget::paintGL()
{
    /* 设置 OpenGLWidget 控件背景颜色为深青色 */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glClear(GL_COLOR_BUFFER_BIT);  // use方法


    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &control_points[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);  // Antialias the lines
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    /* 重新绑定 VAO */
    glBindVertexArray(VAO);


    /* 绘制三角形 */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* 使用着色器 */
    glUseProgram(shaderProgram);
}
