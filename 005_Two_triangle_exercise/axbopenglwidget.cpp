#include "axbopenglwidget.h"
#include "QDebug"

// 顶点数据
float vertexFirstTriangle[] = {
  -0.9f, -0.5f, 0.0f,
   0.0f, -0.5f, 0.0f,
  -0.45f,  0.5f, 0.0f
};
float vertexSecTriangle[] = {
   0.0f, -0.5f, 0.0f,
   0.9f, -0.5f, 0.0f,
   0.45f,  0.5f, 0.0f
};

// 创建 VAO 和 VBO 对象并且赋予 ID
unsigned int VBOs[2];
unsigned int VAOs[2];

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
const char *fragmentShaderSource_left = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\n\0";

// 着色器程序（链接顶点和片段着色器之后生成的）
unsigned int shaderProgram;
unsigned int shaderProgram_left;

AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）


    // ===================== VAO | VBO =====================
    // VAO 和 VBO 对象赋予 ID
    glGenVertexArrays(2,VAOs);
    glGenBuffers(2, VBOs);


    // 绑定 VAO、VBO 对象
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    /* 为当前绑定到 target 的缓冲区对象创建一个新的数据存储（在 GPU 上创建对应的存储区域，并将内存中的数据发送过去）*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexFirstTriangle), vertexFirstTriangle, GL_STATIC_DRAW);

    /* 告知显卡如何解析缓冲区里面的属性值 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 第一个属性，所以不需要偏移

    // 【重点】开始 VAO 管理的第一个属性值
    glEnableVertexAttribArray(0);  // VAOs[0] 里面的第一个属性值

    // 解绑 VAO 和 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* 第二个三角形 */
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexSecTriangle), vertexSecTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



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


    // ===================== 片段着色器 =====================
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int fragmentShader_left = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_left, 1, &fragmentShaderSource_left, NULL);
    glCompileShader(fragmentShader_left);


    // ===================== 链接着色器 =====================
    /* 链接顶点着色器和片段着色器，并生成最后的着色器[程序] */
    shaderProgram = glCreateProgram();  // 【注意】是 `glCreateProgram`
    glAttachShader(shaderProgram, vertexShader);  // 加入顶点着色器
    glAttachShader(shaderProgram, fragmentShader);  // 加入片段着色器
    glLinkProgram(shaderProgram);  // 链接

    shaderProgram_left = glCreateProgram();
    glAttachShader(shaderProgram_left, vertexShader);
    glAttachShader(shaderProgram_left, fragmentShader_left);
    glLinkProgram(shaderProgram_left);


    /* 删除已经不需要的编译的结果 */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader_left);
}

void AXBOpenGLWidget::resizeGL(int w, int h)
{

}

void AXBOpenGLWidget::paintGL()
{
    /* 设置 OpenGLWidget 控件背景颜色为深青色 */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // set方法【重点】如果没有 initializeGL，目前是一个空指针状态，没有指向显卡里面的函数，会报错
    glClear(GL_COLOR_BUFFER_BIT);  // use方法

    /* 使用着色器 */
    glUseProgram(shaderProgram);  // 【重点】当绘制多种颜色的时候要先使用着色器，不然上色会乱
    /* 重新绑定 VAO */
    glBindVertexArray(VAOs[0]);
    /* 绘制三角形 */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /** 第二个三角形 **/
    glUseProgram(shaderProgram_left);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
