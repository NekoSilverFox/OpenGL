#include <QDebug>
#include "foxopenglwidget.h"

// 顶点数据
//float vertices[] = {
//    // 第一个三角形
//    0.5f, 0.5f, 0.0f,   // 右上角
//    0.5f, -0.5f, 0.0f,  // 右下角
//    -0.5f, 0.5f, 0.0f,  // 左上角
//    // 第二个三角形
//    0.5f, -0.5f, 0.0f,  // 右下角
//    -0.5f, -0.5f, 0.0f, // 左下角
//    -0.5f, 0.5f, 0.0f   // 左上角

float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角 0
    0.5f, -0.5f, 0.0f,  // 右下角 1
    -0.5f, -0.5f, 0.0f, // 左下角 2
    -0.5f, 0.5f, 0.0f   // 左上角 3
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

// 着色器程序（链接顶点和片段着色器之后生成的）
unsigned int shaderProgram;


FoxOpenGLWidget::FoxOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}


/* 首要要执行初始化过程，将函数指针指向显卡内的函数 */
void FoxOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 第一个属性，所以不需要偏移


    // 开始 VAO 管理的第一个属性值
    glEnableVertexAttribArray(0);

    // ===================== EBO =====================
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // EBO/IBO 是储存顶点【索引】的


    // 解绑 VAO 和 VBO，注意先解绑 VAO再解绑EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 注意 VAO 不参与管理 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



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
    shaderProgram = glCreateProgram();  // 【注意】是 `glCreateProgram()`
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

    /* 绘制三角形 */
//    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 代表6个点，因为一个矩形是2个三角形构成的，一个三角形有3个点
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);  // 直接到索引数组里去绘制，如果VAO没有绑定EB的话



    /* 使用着色器 */
    glUseProgram(shaderProgram);
}
