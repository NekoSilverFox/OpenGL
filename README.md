<p align="center">
 <img width="100px" src="https://raw.githubusercontent.com/NekoSilverFox/NekoSilverfox/403ab045b7d9adeaaf8186c451af7243f5d8f46d/icons/silverfox.svg" align="center" alt="NekoSilverfox" />
 <h1 align="center">OpenGL</h1>
 <p align="center"><b>计算机图形</b></p>
</p>



<div align=center>


[![License](https://img.shields.io/badge/license-Apache%202.0-brightgreen)](LICENSE)



<div align=left>
<!-- 顶部至此截止 -->
[toc]




# 简介及资料

**OpenGL 简介：**

OpenGL 是一种协议和标准，是一种小而美的东西。因为它是一种框协议，所以它是不需要管 OS 层面和不同显卡层面的差异的。

OpenGL 官方的推荐为：GLFW+GLAD，但是我们可以使用 OpenGL+Qt（MinGW）。借助于 Qt 的封装，我们无需配置第三方库，更接近于实战



**LearnOpenGL 中文版网站：**

https://learnopengl-cn.github.io/



**视频教程：**

https://ke.qq.com/course/package/40726?flowToken=1041265



**版本的使用：**

- OpenGL3.3 及以上版本，因为具备可编程管线的功能。3.3 版本是一个分水岭
- VC++ 最好使用 2013 以上的版本
- C++11 之上的版本



**Cmakelist 的编写：**

基于 M1 Mac

```cmake
cmake_minimum_required(VERSION 3.22)
project(OpenGL)

set(CMAKE_CXX_STANDARD 11)

# 包含头文件
include_directories(./glad/include)

# brew 安装的库都在下面的目录里（M1 版 Mac） /opt/homebrew/Cellar/
# 把 glfw 加进项目
include_directories(/opt/homebrew/Cellar/glfw/3.3.6/include)

# glfw 库文件也加进来
link_directories(/opt/homebrew/Cellar/glfw/3.3.6/lib)

add_executable(OpenGL  # 这个是工程的名字
        ./glad/src/glad.c
        main.cpp)

# 最后把库文件 link 到项目
target_link_libraries(
        OpenGL  # 这个是工程的名字
        glfw
)
```



# OpenGL 对象

一个对象（Object）是指一些选项的集合，**代表OpenGL状态的一个子集**，可以使用对象来记录状态

通常把0penGL上下文比作一个大的结构体，包含很多子集

![image-20220729182054940](doc/pic/README/image-20220729182054940.png)





【重点】**示例：**

```cpp
// 1. 创建对象
GLuint objectId = 0;  // 创建一个 ID（GLuint 是 OpenGL 自己的 int 类型）
glGenObject(1, &objectId);  // 生成了一个对象（小助理），并给他了一个编号

// 2. 绑定至上下文对象
glBindObject(GL_WINDOW_TARGET, objectId);  // 让这个对象【记录】状态机中子集 GL_WINDOW_TARGET 的状态，也就是确定工作内容

// 3. 设置子集 GL_WINDOW_TARGET 的一些选项，以下两条的操作会被 objectId对象 记录
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);  // 设置窗口宽度
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);  // 设置窗口高度

// 4. 将上下文的 GL_WINDOW_TARGET 设回默认
glBindObject(GL_WINDOW_TARGET, 0);  // 将objectId对象与GL_WINDOW_TARGET解绑；但 objectId对象 已经记录了上面的两条内容。需要查看记录的时候再喊他过来就好。这里的 0 代表空对象，即为不绑定任何对象

// 5. 一旦重新绑定 objectId对象 到 GL_WINDOW_TARGET，这些选项会重新生效
```

![image-20220808195151096](doc/pic/README/image-20220808195151096.png)

- 在进行使用 QOpenGLWidget 时，要首先要在初始化函数`initializeGL()` 中调用 `QOpenGLFunctions_X_X_Core` 中的 init 将 Qt 里的函数指针指向显卡的函数（下图所示）





![image-20220808195106987](doc/pic/README/image-20220808195106987.png)



# 你好，三角形

<img src="doc/pic/README/image-20220812134100244.png" alt="image-20220812134100244" style="zoom:50%;" />

顶点数据（`Vertex DATA[]`）的传递 -> 我们要将内存里的数据传递到显卡里

1. **顶点着色器**（自己写）

    - 它会在 GPU 上创建内存，用于储存我们的顶点数据（将数据从内存读取到显存，一个缓冲区一个缓冲区的填充）
    - 通过**顶点缓冲对象 `Vertex Buffer Objects, VBO`**管理，顶点缓冲对象的缓冲类型是 `GL_ARRAY_BUFFER`
    - 配置 OpenGL 如何解释这些内存：通过**顶点数组对象 `Vertex Array Objects, VAO`** 管理，**VAO 的数据类型是唯一的，数组里的每一个项都对应一个属性的解析（类似一个类 | 结构体）**。VAO 并不保存实际的数据，而是放顶点结构的定义
    - OpenGL 允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型

    <img src="doc/pic/README/image-20220812155627459.png" alt="image-20220812155627459" style="zoom:50%;" />

    ```c++
    // 顶点数据
    float vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };
    
    // 创建 VAO 和 VBO 对象并且赋予 ID
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);  // Array 存放数据结构的定义
    glGenBuffers(1, &VBO);  // Buffer 缓冲区才是真正存放顶点数据的
    
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
    glEnableVertexAttribArry(0);
    
    // 解绑 VAO 和 VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ```

    

2. 形状（图元）装配

3. **几何着色器**（可以自己写也可以不写）

4. 光栅化

5. **片段着色器**（自己写）

6. 测试与混合



**标准化设备坐标**：(Normalized Device Coordinates, NDC) 顶点着色器中处理过后，就应该是标准化设备坐标了，x、y和Z的值在-1.0到1.0的一小段空间（立方体）。 落在范围外的坐标都会被裁剪，因为 GPU 没有必要再去消耗算力去进行计算



















