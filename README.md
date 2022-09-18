<p align="center">
 <img width="100px" src="https://raw.githubusercontent.com/NekoSilverFox/NekoSilverfox/403ab045b7d9adeaaf8186c451af7243f5d8f46d/icons/silverfox.svg" align="center" alt="NekoSilverfox" />
 <h1 align="center">OpenGL</h1>
 <p align="center"><b>计算机图形</b></p>
</p>

> “绝不能恐惧，恐惧是思维杀手。恐惧是引向毁灭的死神。我将正视恐惧，任由它穿过我的躯体。当恐惧逝去，我会洞悉它的轨迹。恐惧所过之处，万物无存，唯我独立。”


<div align=center>



[![License](https://img.shields.io/badge/license-Apache%202.0-brightgreen)](LICENSE)



<div align=left>
<!-- 顶部至此截止 -->
[toc]



# 目录说明

| 编号                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [000 - Apple M1 配置测试](https://github.com/NekoSilverFox/OpenGL/tree/main/000 - Apple M1 配置测试) | 在 M1 Mac 下 OpenGL 的配置及运行（基于 Clang）               |
| [001_OpenGLWidget_HelloWidget](https://github.com/NekoSilverFox/OpenGL/tree/main/001_OpenGLWidget_HelloWidget) | 如果在 Qt 的环境下，使用初始化 OpenGL 控件                   |
| [002_Triangle_VAO_VBO](https://github.com/NekoSilverFox/OpenGL/tree/main/002_Triangle_VAO_VBO) | 基于 `001` 的初始化控件后，增加使用 VAO、VBO 代码绘制三角形的代码<br />顶点和片段着色器的编写、编译、链接和使用 |
| [003_Triangle_EBO](https://github.com/NekoSilverFox/OpenGL/tree/main/003_Triangle_EBO) | 通过 EBO（元素/索引缓冲对象）进行了 2 个三角形的绘制（绘制为 1 个矩形） |
| [005_Two_triangle_exercise](https://github.com/NekoSilverFox/OpenGL/tree/main/005_Two_triangle_exercise) | 小练习：使用 VAO 和 VBO 数组进行对不同三角形的绘制；使用 2 个片段着色器进行分别着色 |
| [010_Qt_UI_with_OpenGL](https://github.com/NekoSilverFox/OpenGL/tree/main/010_Qt_UI_with_OpenGL) | Qt UI 调用 OpenGL 功能：通过 QOpenGLWidget 上的按钮实现三角形的绘制、清除、是否开启线框模式<br />Qt`update()` 函数的注意事项及使用 |
| [011_QOpenGLShaderProgram](https://github.com/NekoSilverFox/OpenGL/tree/main/011_QOpenGLShaderProgram) | 使用 Qt 提供的 QOpenGLShaderProgram 类，进行顶点和片段着色器的编译链接和使用。并且将 GLSL 代码以 Qt 资源文件的方式传入和使用 |
|                                                              |                                                              |
|                                                              |                                                              |
|                                                              |                                                              |
|                                                              |                                                              |



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



# 专有名词及解释

| 名词                            | 英文                          | 解释                                                         |
| ------------------------------- | ----------------------------- | ------------------------------------------------------------ |
| 对象                            | Object                        | 一个对象是指一些选项的集合，**代表OpenGL状态的一个子集**，可以使用对象来记录状态 |
| 图形渲染管线                    | Graphics Pipeline             | 实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程。图形渲染管线可以被划分为两个主要部分：第一部分把你的3**D坐标转换为2D坐标**，第二部分是把**2D坐标转变为实际的有颜色的像素** |
| 着色器                          | Shader                        | 在GPU上为每一个（渲染管线）阶段运行的小程序（小核心）        |
| 图元                            | Primitive                     | 指定坐标和颜色值构成的是什么，这些数据所表示的渲染类型；是渲染成一个点（GL_POINT）、一条线（GL_LINE_STRIP）还是三角形（GL_TRIANGLES） |
| 片段                            | Fragment                      | 在 OpenGL 中一个片段是 OpenGL 渲染一个像素所需要的所有数据   |
| 裁切                            | Clipping                      | 裁切会丢弃超出视图以外的所有像素，用来提升执行效率           |
| 深度                            |                               | 可以理解为z坐标，它代表一个像素在空间中和你的距离，如果离你远就可能被别的像素遮挡，你就看不到它了，它会被丢弃，以节省资源 |
| NDC 标准化设备坐标              | Normalized Device Coordinates | 顶点着色器中处理过后，就是标准化设备坐标了，x、y、z的值在-1.0到1.0的一小段空间（立方体）。 落在范围外的坐标都会被裁剪，因为 GPU 没有必要再去消耗算力去进行计算 |
| 顶点着色器                      | Vertex Shader                 | **图形渲染管线的第一部分**，它把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标，同时允许我们对顶点属性进行一些基本处理 |
| ↓ 图元装配                      | Primitive Assembly            | 图元装配阶段将顶点着色器输出的所有顶点作为输入（如果是GL_POINTS，那么就是一个顶点），并**将所有的点装配成指定图元的形状** |
| ↓ 几何着色器                    | Geometry Shader               | 图元装配阶段的输出会传递给几何着色器(Geometry Shader)。几何着色器把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状 |
| ↓ 光栅化阶段                    | Rasterization Stage           | 几何着色器的输出会被传入光栅化阶段，里它会把**图元映射为最终屏幕上相应的像素**，生成供片段着色器(Fragment Shader)使用的==片段==。在片段着色器运行之前会执行裁切(Clipping) |
| ↓ 片段着色器                    | Fragment Shader               | 计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。 |
| ↓ Alpha测试和混合(Blending)阶段 |                               | 这个阶段检测片段的对应的深度（和模板(Stencil)）值，用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个图形时最后的像素颜色也可能完全不同 |
| 顶点                            | Vertex                        | 一个3D坐标的数据集合                                         |
| 顶点数据                        | Vertex Data                   | 顶点数据是一系列顶点的集合                                   |
| 顶点属性                        | Vertex Attribute              | 顶点的数据是用顶点属性表示的，它可以包含任何我们想用的数据   |
| VAO 顶点数组对象                | Vertex Array Object           | 使用 VAO 配置 OpenGL 如何解释显存，VAO 的数据类型是唯一的，并不保存实际的数据，而是**放顶点结构的定义** |
| VBO 顶点缓冲对象                | Vertex Buffer Object          | VBO 顶点缓冲对象**管理着**顶点着色器（Vertex Shader），VBO的**类型是**`GL_ARRAY_BUFFER`；它会在 GPU 上创建内存，用于储存我们的大量顶点数据 |
| EBO/IBO 元素/索引缓冲对象       | Element/Index Buffer Object   | EBO是一个缓冲区，就像一个顶点缓冲区对象一样，它**存储 OpenGL 要绘制顶点的==索引==** |
| 着色器程序对象                  | Shader Program Object         | 多个着色器链接（Link）合并之后并最终链接完成的版本，在渲染对象时使用`glUseProgram(shaderProgram)`激活这个着色器程序 |





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

- 在进行使用 QOpenGLWidget 时，要首先要在初始化函数`initializeGL()` 中调用 `QOpenGLFunctions_X_X_Core::` 中的 init **将 Qt 里的函数指针指向显卡的函数（下图所示）**



![image-20220808195106987](doc/pic/README/image-20220808195106987.png)



# 你好，三角形

<img src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b96de0784ceb4b7d8aee18e2096bba70~tplv-k3u1fbpfcp-watermark.awebp" alt="draw" style="zoom:50%;" />

> https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
>
> 在学习此节之前，建议将这三个单词先记下来：
>
> - 顶点数组对象：Vertex Array Object，VAO
> - 顶点缓冲对象：Vertex Buffer Object，VBO
> - 元素缓冲对象：Element Buffer Object，EBO 或 索引缓冲对象 Index Buffer Object，IBO

在OpenGL中，任何事物都在3D空间中，而屏幕和窗口却是2D像素数组，这导致OpenGL的大部分工作都是关于把3D坐标转变为适应你屏幕的2D像素。3D坐标转为2D坐标的处理过程是由OpenGL的**图形渲染管线**（Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程）管理的。图形渲染管线可以被划分为两个主要部分：第一部分把你的3D坐标转换为2D坐标，第二部分是把2D坐标转变为实际的有颜色的像素。



*2D坐标和像素也是不同的，2D坐标精确表示一个点在2D空间中的位置，而2D像素是这个点的近似值，2D像素受到你的屏幕/窗口分辨率的限制。*



图形渲染管线接受一组3D坐标，然后把它们转变为你屏幕上的有色2D像素输出。图形渲染管线可以被划分为几个阶段，每个阶段将会把前一个阶段的输出作为输入。所有这些阶段都是高度专门化的（它们都有一个特定的函数），并且很容易并行执行。正是由于它们具有并行执行的特性，当今大多数显卡都有成千上万的小处理核心，它们在GPU上为每一个（渲染管线）阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些小程序叫做**着色器(Shader)。**



有些着色器可以由开发者配置，因为允许用自己写的着色器来代替默认的，所以能够更细致地控制图形渲染管线中的特定部分了。因为它们运行在GPU上，所以节省了宝贵的CPU时间。**OpenGL着色器是用OpenGL着色器语言(OpenGL Shading Language, GLSL)写成的**





![img](https://learnopengl-cn.github.io/img/01/04/pipeline.png)

1. 图形渲染管线的第一个部分是**==顶点着色器(Vertex Shader)==**，它把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标（后面会解释），同时顶点着色器允许我们对顶点属性进行一些基本处理



2. **==图元装配(Primitive Assembly)==阶段将顶点着色器输出的所有顶点作为输入（如果是GL_POINTS，那么就是一个顶点），并所有的点装配成指定图元的形状**；本节例子中是一个三角形。

    为**了让OpenGL知道我们的坐标和颜色值构成的到底是什么，OpenGL需要你去指定这些数据所表示的渲染类型**。我们是希望把这些数据渲染成一系列的点？一系列的三角形？还是仅仅是一个长长的线？**做出的这些提示叫做==图元(Primitive)==**，任何一个绘制指令的调用都将把图元传递给OpenGL。**这是其中的几个：GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP。**



3. **图元装配阶段的输出会传递给==几何着色器(Geometry Shader)==。几何着色器把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。**例子中，它生成了另一个三角形。



4. 几何着色器的输出会被传入**==光栅化阶段(Rasterization Stage)==**，**这里它会把图元映射为最终屏幕上相应的像素，生成供片段着色器(Fragment Shader)使用的片段(Fragment | OpenGL中的一个片段是OpenGL渲染一个像素所需的所有数据)。在片段着色器运行之前会执行裁切(Clipping)。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。**

5. **==片段着色器==的主要目的是计算一个像素的最终颜色**，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。



6. 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做**==Alpha测试和混合(Blending)阶段==**。这个阶段检测片段的对应的深度（和模板(Stencil)）值（后面会讲），用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。

    通常==**深度**==可以理解为z坐标，它代表一个像素在空间中和你的距离，如果离你远就可能被别的像素遮挡，你就看不到它了，它会被丢弃，以节省资源。



可以看到，图形渲染管线非常复杂，它包含很多可配置的部分。然而，**对于大多数场合，我们只需要配置顶点和片段着色器就行了**。几何着色器是可选的，通常使用它默认的着色器就行了。

在现代OpenGL中，我们**必须**定义至少一个顶点着色器和一个片段着色器（因为GPU中没有默认的顶点/片段着色器）。出于这个原因，刚开始学习现代OpenGL的时候可能会非常困难，因为在你能够渲染自己的第一个三角形之前已经需要了解一大堆知识了。



**下面，你会看到一个图形渲染管线的每个阶段的抽象展示。要注意蓝色部分代表的是我们可以注入自定义的着色器的部分。**



<img src="doc/pic/README/image-20220812134100244.png" alt="image-20220812134100244" style="zoom:50%;" />



## VAO、VBO

首先，我们以数组的形式传递3个3D坐标作为图形渲染管线的输入，用来表示一个三角形，这个数组叫做**顶点数据(Vertex Data)；顶点数据是一系列顶点的集合。一个顶点(Vertex)是一个3D坐标的数据的集合。而顶点数据是用顶点属性(Vertex Attribute)表示的，它可以包含任何我们想用的数据**，但是简单起见，我们还是假定每个顶点只由一个3D位置(译注1)和一些颜色值组成的吧。





顶点数据（`Vertex DATA[]`）的传递 -> 我们要将内存里的数据传递到显卡里

1. **顶点着色器**（自己写在 `XXXOpenGLWedget::initializeGL()` 中）

    - 通过**顶点缓冲对象 `Vertex Buffer Objects, VBO`**管理，顶点缓冲对象的缓冲**类型是** `GL_ARRAY_BUFFER`。它会在 GPU 上创建内存，用于储存我们的大量顶点数据（将数据从内存读取到显存，一个缓冲区一个缓冲区的填充）。使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。从CPU把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据。当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程。

      ​    

    - OpenGL 允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型

      ​    

    - 配置 OpenGL 如何解释这些内存：通过**顶点数组对象 `Vertex Array Objects, VAO`** 管理，**VAO 的数据类型是唯一的，数组里的每一个项都对应一个属性的解析（类似一个类 | 结构体）**。的

        顶点**数组**对象(Vertex Array Object, VAO)可以像顶点**缓冲**对象（VBO）那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。

        > **OpenGL的核心模式**要求**我们使用VAO，所以它知道该如何处理我们的顶点输入。如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。**

        

        一个顶点数组对象会储存以下这些内容：

        - `glEnableVertexAttribArray`和`glDisableVertexAttribArray`的调用。

        - 通过`glVertexAttribPointer`设置的顶点属性配置。

        - 通过`glVertexAttribPointer`调用与顶点属性关联的顶点缓冲对象

          ​    

        要想使用VAO，要做的只是使用`glBindVertexArray`绑定VAO。从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。当我们打算绘制一个物体的时候，我们只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了。这段代码应该看起来像这样：

        ```c++
        // ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
        // 1. 绑定VAO
        glBindVertexArray(VAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 3. 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        [...]
        
        // ..:: 绘制代码（渲染循环中） :: ..
        // 4. 绘制物体
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        someOpenGLFunctionThatDrawsOurTriangle();
        ```

        就这么多了！前面做的一切都是等待这一刻，一个储存了我们顶点属性配置和应使用的VBO的顶点数组对象。一般当你打算绘制多个物体时，你首先要生成/配置所有的VAO（和必须的VBO及属性指针)，然后储存它们供后面使用。**当我们打算绘制物体的时候就拿出相应的VAO，绑定它，绘制完物体后，再解绑VAO。**

        

    <img src="doc/pic/README/image-20220812155627459.png" alt="image-20220812155627459" style="zoom:50%;" />

    ```c++
    // 顶点数据，由于OpenGL是在3D空间中工作的，而我们渲染的是一个2D三角形，我们将它顶点的z坐标设置为0.0
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
    glBindVertexArray(VAO);  // 【重点】VAO会储存glBindBuffer的函数调用
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 新创建的缓冲绑定到GL_ARRAY_BUFFER目标上，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
    
    
    /* glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。第三个参数是我们希望发送的实际数据。
    	为当前绑定到 target 的缓冲区对象创建一个新的数据存储（在 GPU 上创建对应的存储区域，并将内存中的数据发送过去）
    	如果 data 不是 NULL，则使用来自此指针的数据初始化数据存储
    	void glBufferData(GLenum target,  // 目标缓冲的类型，需要在 GPU 上创建的目标
    										GLsizeipter size,  // 创建的显存大小(以字节为单位)
    										const GLvoid* data,  // 希望发送的实际数据
    										GLenum usage)  // 创建在 GPU 上的哪一片区域（显存上的每个区域的性能是不一样的）https://registry.khronos.org/OpenGL-Refpages/es3.0/
    										GL_STATIC_DRAW ：数据不会或几乎不会改变。
    										GL_DYNAMIC_DRAW：数据会被改变很多。
    										GL_STREAM_DRAW ：数据每次绘制时都会改变。
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 现在我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理
    
    /* 告知显卡如何解析缓冲区里面的属性值（设置顶点属性指针）
    	void glVertexAttribPointer(
    															GLuint index,  // VAO 中的第几个属性（VAO 属性的索引），我们在顶点着色器中使用layout(location = 0)定义了
    															GLint size,  // 顶点属性的大小，VAO 中的第几个属性中对应的位置放几份数据，顶点属性是一个vec3，它由3个值组成，所以大小是3
    															GLEnum type,  // 存放数据的数据类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)
    															GLboolean normalized,  // 是否标准化(Normalize)，如设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
    															GLsizei stride,  // 步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）
    															const void* offset  // 位置数据在缓冲中起始位置的偏移量
    	)
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 第一个属性，所以不需要偏移
    
    // 开始 VAO 管理的第一个属性值
    glEnableVertexAttribArray(0);
    
    // 解绑 VAO 和 VBO ???将两行反过来写，试试还能不能绘制???
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ```

    **编写完上述代码之后，如果要使用的话，需要在 `XXXOpenGLWedget::paintGL()` 中重新绑定 VAO 和 VBO：**

    ```c++
    glBindVertexArray(VAO);
    ```

    

    

2. 形状（图元）装配

3. **几何着色器**（可以自己写也可以不写）

4. 光栅化

5. **片段着色器**（自己写）

6. 测试与混合



**标准化设备坐标**：(Normalized Device Coordinates, NDC) 顶点着色器中处理过后，就应该是标准化设备坐标了，x、y和Z的值在-1.0到1.0的一小段空间（立方体）。 落在范围外的坐标都会被裁剪，因为 GPU 没有必要再去消耗算力去进行计算



## 编译着色器

> https://www.imgeek.org/article/825358359

**渲染管线的作用是将3D模型转换为2维图像。**

- **顶点着色器**

    就是和GPU打交道，在GPU上运行的代码是一对着色器，一个是顶点着色器，另一个是片段着色器。每次调用着色程序都会先执行顶点着色器，再执行片元着色器。

    一个顶点着色器的工作是生成裁剪空间坐标值，是用着色器语言GLSL(OpenGL Shading Language)编写：

    ```c++
    #version 330 core
    layout (location = 0) in vec3 aPos;
    
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
    ```

    可以看到，GLSL看起来很像C语言。每个着色器都起始于一个版本声明。OpenGL 3.3以及和更高版本中，GLSL版本号和OpenGL的版本是匹配的（比如说GLSL 420版本对应于OpenGL 4.2）。我们同样明确表示我们会使用核心模式。

    

    下一步，使用`in`关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性。GLSL有一个向量数据类型，它包含1到4个`float`分量，包含的数量可以从它的后缀数字看出来。由于每个顶点都有一个3D坐标，我们就创建一个`vec3`输入变量aPos。我们同样也通过`layout (location = 0)`设定了输入变量的位置值(Location)你后面会看到为什么我们会需要这个位置值。

    

    > **向量(Vector)**
    >
    > 在图形编程中我们经常会使用向量这个数学概念，因为它简明地表达了**任意空间中的位置和方向**，并且它有非常有用的数学属性。在GLSL中一个向量有最多4个分量，每个分量值都代表空间中的一个坐标，它们可以通过`vec.x`、`vec.y`、`vec.z`和`vec.w`来获取。注意`vec.w`分量不是用作表达空间中的位置的（我们处理的是3D不是4D），而是用在所谓透视除法(Perspective Division)上。我们会在后面的教程中更详细地讨论向量。

    

    每个顶点调用一次（顶点）着色器，每次调用都需要设置一个特殊的全局变量 **gl_Position**。 该变量的值就是裁减空间坐标值。何为裁剪空间坐标？就是无论你的画布有多大，裁剪坐标的坐标范围永远是 -1 到 1 。

    <img src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/71c4328e45884accbf3138cbc9336d99~tplv-k3u1fbpfcp-watermark.awebp" alt="裁剪坐标系" style="zoom:30%;" />

    如果运行一次顶点着色器， 那么gl_Position 就是 **（-0.5，-0.5，0，1）** 记住他永远是个 **Vec4**, 简单理解就是对应**x、y、z、w**。即使你没用其他的，也要设置默认值， 这就是所谓的 3维模型转换到我们屏幕中。

    

    ---

    

    **顶点着色器的编译过程：**

    这个过程类似于 CPP 的（编写、编译、链接）过程；我们通过在 CPP 中编写、编译、链接面向 GPU 的代码

    1. **编写源码**（最好作为全局或成员变量）

        现在，我们暂时将顶点着色器的*GLSL源代码*硬编码在代码文件顶部的C风格字符串中：

        ```c++
        // 【原码】顶点着色器就是把 xyz 原封不动的送出去
        const char* vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        ```

        为了能够让OpenGL使用它，我们必须在**运行时动态编译它的GLSL源代码**。

    1. **编译顶点着色器**（写在 `XXXOpenGLWedget::initializeGL()` 中）

        ```c++
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);  // 创建顶点着色器（框架 | 对象）并给予编号，由于我们正在创建一个【顶点】着色器，传递的参数是GL_VERTEX_SHADER。
        
        /* 绑定至着色器原码
        	void glShaderSource(
        											GLuint shader,  要着色器框架 | 对象
        											GLsize count,  着色器字符串的数量
        											const CLchar** string,  着色器源码字符串
        											const GLint* length  着色器源码的长度，如果是单个字符串可以填 NULL（代表源码字符串以 NULL 结尾）
        	)
        
        */
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  // 绑定至着色器原码
        
        glCompileSharder(vertexShader);  // 编译着色器
        
        /* 因为可能出错，所以进行错误检查，也就是判断时候成功编译 */
        int success;  // 是否成功的标志
        char infolog[512];  // 错误日志（信息）
        glGetShaderiv(vertexShader, GL_COMPILES_TATUS, &success);  // 检查是否编译成功
        if(!success)
        {
          glGetShaderInfoLog(vertexShader, 512, NULL, infolog);  // 获取错误消息
          qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog;
        }
        ```

        

- **片段着色器**

    片段着色器(Fragment Shader)是第二个也是最后一个我们打算创建的用于渲染三角形的着色器。片段着色器所做的是计算像素最后的颜色输出。

    > 在计算机图形中颜色被表示为有4个元素的数组：红色、绿色、蓝色和alpha(透明度)分量，通常缩写为RGBA。当在OpenGL或GLSL中定义一个颜色的时候，我们把颜色每个分量的强度设置在0.0到1.0之间。比如说我们设置红为1.0f，绿为1.0f，我们会得到两个颜色的混合色，即黄色。这三种颜色分量的不同调配可以生成超过1600万种不同的颜色！

    

    1. **编写源码**

        片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，我们应该自己将其计算出来。声明输出变量可以使用`out`关键字，这里我们命名为`FragColor`。下面，我们将一个Alpha值为1.0(1.0代表完全不透明)的橘黄色的`vec4`赋值给颜色输出。

    ```c++
    // 【原码】片段着色器就是给一个固定的颜色
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";
    ```

    

    2. **编译片段着色器**（写在 `XXXOpenGLWedget::initializeGL()` 中）

    过程跟编译顶点着色器的框架是一样的，用的是同一套函数

    ```c++
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  // 创建片段着色器（框架 | 对象）并给予编号
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  // 绑定至着色器原码
    glCompileShader(fragmentShader);  // 编译着色器
    
    /* 因为可能出错，所以进行错误检查，也就是判断时候成功编译 */
    // int success;  // 是否成功的标志
    // char infolog[512];  // 错误日志（信息）
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
      qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog;
    }
    ```

    两个着色器现在都**编译**了，剩下的事情是把两个着色器对象**==链接==**到一个用来渲染的**着色器程序(Shader Program)**中。



## 链接着色器为着色器程序

在进行完上面一节的着色器编写和编译后，需要**将顶点着色器和片段着色器进行链接*（就像 CPP 生成可执行文件的顺序那样：编写(.cpp) ->编译(.o) ->链接(.exe)）***

**着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本**。如果**要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序**。已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。

当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。当输出和输入不匹配的时候，你会得到一个连接错误

```c++
/* 链接顶点着色器和片段着色器，并生成最后的着色器程序 */
unsigned int shaderProgram = glCreateProgram();  // 注意，这里是 `glCreateProgram()`，因为链接之后生成的便是最后的程序
glAttahcShader(shaderProgram, vertexShader);  // 加入顶点着色器
glAttachShader(shaderProgram, fragmentShader);  // 加入片段着色器
glLinkProgram(shaderProgram);  // 链接

/* 因为可能出错，所以进行错误检查，也就是判断时候成功链接 */
// int success;  // 是否成功的标志
// char infolog[512];  // 错误日志（信息）
glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
if(!success)
{
  glGetShaderInfoLog(shaderProgram, 512, NULL, infolog);
  qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog;
}

/* 在把着色器对象链接到程序对象以后，删除已经不需要的编译的结果 */
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```



得到的结果就是一个程序对象，我们可以调用`glUseProgram`函数，用刚创建的程序对象作为它的参数，以激活这个程序对象：

*在 `XXXOpenGLWedget::paintGL()` 中使用（开启）着色器*

```c++
glUseProgram(shaderProgram);
```

在`glUseProgram`函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。



现在，我们已经把输入顶点数据发送给了GPU，并指示了GPU如何在顶点和片段着色器中处理它。



## 绘制三角形

要想绘制我们想要的物体，OpenGL给我们提供了`glDrawArrays`函数，**它使用当前激活的着色器，之前定义的顶点属性配置，和VBO的顶点数据（通过VAO间接绑定）来绘制图元。**

```c++
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

`glDrawArrays()`函数：

- 第一个参数是我们打算绘制的**OpenGL图元的类型**。由于我们在一开始时说过，我们希望绘制的是一个三角形，这里传递`GL_TRIANGLES`给它。
- 第二个参数指定了顶点数组的起始索引，我们这里填`0`。
- 最后一个参数指定我们打算**绘制多少个顶点**，这里是`3`（我们只从我们的数据中渲染一个三角形，它只有3个顶点长）。

现在尝试编译代码，如果弹出了任何错误，回头检查你的代码。如果你编译通过了，你应该看到下面的结果：

![img](https://learnopengl-cn.github.io/img/01/04/hellotriangle.png)





## EBO 元素缓冲对象

元素缓冲对象(Element Buffer Object，EBO)，也叫索引缓冲对象(Index Buffer Object，IBO)

假设我们不再绘制一个三角形而是绘制一个矩形。我们可以绘制两个三角形来组成一个矩形（OpenGL主要处理三角形）。这会生成下面的顶点的集合：

```c++
float vertices[] = {
    // 第一个三角形
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, 0.5f, 0.0f,  // 左上角
    // 第二个三角形
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};
```

可以看到，有几个顶点叠加了。我们指定了`右下角`和`左上角`两次！一个矩形只有4个而不是6个顶点，这样就产生50%的额外开销。当我们有包括上千个三角形的模型之后这个问题会更糟糕，这会产生一大堆浪费。更好的解决方案是只储存不同的顶点，并设定绘制这些顶点的顺序。这样子我们只要储存4个顶点就能绘制矩形了，之后只要指定绘制的顺序就行了。如果OpenGL提供这个功能就好了，对吧？

值得庆幸的是，元素缓冲区对象的工作方式正是如此。 **EBO是一个缓冲区，就像一个顶点缓冲区对象一样，它存储 OpenGL 要绘制顶点的==索引==**。这种所谓的**索引绘制(Indexed Drawing)**正是我们问题的解决方案。首先，我们先要定义（不重复的）顶点，和绘制出矩形所需的索引：

```c++
float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
```

你可以看到，当使用索引的时候，我们只定义了4个顶点，而不是6个。下一步我们需要创建元素缓冲对象：

```c++
unsigned int EBO;
glGenBuffers(1, &EBO);
```

与VBO类似，我们先绑定EBO然后用glBufferData把索引复制到缓冲里。同样，和VBO类似，我们会把这些函数调用放在绑定和解绑函数调用之间，只不过这次我们把缓冲的类型定义为GL_ELEMENT_ARRAY_BUFFER。

```c++
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

注意：我们传递了GL_ELEMENT_ARRAY_BUFFER当作缓冲目标。最后一件要做的事是用glDrawElements来替换glDrawArrays函数，表示我们要从索引缓冲区渲染三角形。使用glDrawElements时，我们会使用当前绑定的索引缓冲对象中的索引进行绘制：

```c++
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

- 第一个参数指定了我们**绘制的模式**，这个和glDrawArrays的一样。
- 第二个参数是我们打算**绘制顶点的个数**，这里填6，也就是说我们一共需要绘制6个顶点。
- 第三个参数是**索引的类型**，这里是GL_UNSIGNED_INT。
- 最后一个参数里我们可以指定**EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0。**

`glDrawElements`函数从当前绑定到`GL_ELEMENT_ARRAY_BUFFER`目标的EBO中获取其索引。这意味着我们每次想要使用索引渲染对象时都必须绑定相应的EBO，这又有点麻烦。碰巧顶点数组对象也跟踪元素缓冲区对象绑定。在绑定VAO时，绑定的最后一个元素缓冲区对象存储为VAO的元素缓冲区对象。然后，绑定到VAO也会自动绑定该EBO。

![img](https://learnopengl-cn.github.io/img/01/04/vertex_array_objects_ebo.png)

**当目标是`GL_ELEMENT_ARRAY_BUFFER`的时候，VAO会储存`glBindBuffer`的函数调用。这也意味着它也会储存解绑调用，所以确保你没有在解绑VAO之前解绑索引数组缓冲，否则它就没有这个EBO配置了。**但是 VAO 不参与 VBO 的管理，所有一定要注意 VAO 和 EBO 的解绑顺序（先解绑 VAO，再解绑 EBO）

最后的初始化和绘制代码现在看起来像这样：

```c++
// ..:: 初始化代码 :: ..
// 1. 绑定顶点数组对象
glBindVertexArray(VAO);
// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 4. 设定顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

[...]

// ..:: 绘制代码（渲染循环中） :: ..
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
```

运行程序会获得下面这样的图片的结果。左侧图片看应该起来很熟悉，而右侧的则是使用线框模式(Wireframe Mode)绘制的。线框矩形可以显示出矩形的确是由两个三角形组成的。

![img](https://learnopengl-cn.github.io/img/01/04/hellotriangle2.png)

**线框模式(Wireframe Mode)**

要想用线框模式绘制你的三角形，你可以通过`glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)`函数配置OpenGL如何绘制图元。第一个参数表示我们打算将其应用到所有的三角形的正面和背面，第二个参数告诉我们用线来绘制。之后的绘制调用会一直以线框模式绘制三角形，直到我们用`glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)`将其设置回默认模式。



# Qt 交互

- 如果需要从 `paintGL()` 以外的位置触发重新绘制（比如使用计时器设置场景动画），则应调用 widget 的 `update()` 函数来安排更新
- 调用 `paintGL()`、`resizeGL()` 或 `initializeGL()` 时，widget 的 OpenGL 呈现上下文将变为当前。如果需要从其他位置（例如：在 widget 的构造函数或自己的绘制函数中）调用标准 OpenGL API 函数，则必须首先调用 `makeCurrent()`



> **在 `paintGL()` 以外的地方调用绘制函数，没有意义。绘制图形最终将会被 `paintGL()` 覆盖**
>
> 我们在写自己函数的时候，可以采用一下结构：
>
> 【注意】只在 `paintGL()` 外部，且**中间是代码调用了 OpenGL 功能时**才这么写
>
> ```c++
> ...
> makeCurrent();
> 
> 	... 自己写的调用了 OpenGL 功能的代码 ...
> 
> 
> doneCurrent();
> 	update();  执行重绘，这条语句会重新执行 paintGL()
> ...
> ```
>
> **`update();`  会执行重绘，也就是这条语句会==重新调用== `paintGL()`**



## QOpenGLShaderProgram

可以通过 Qt 提供的 `QOpenGLShaderProgram` 对象进行 GLSL 源码（着色器）的编译和链接

在同名的头文件 `#include <QOpenGLShaderProgram>`中，提供的方法有：

- **添加（顶点 | 片段）着色器原码：**

    - 通过**源码GLSL字符串**添加：

        `QOpenGLShaderProgram.addShaderFromSourceCode(QOpenGLShader::源码类型, 源码);`

        

    - 通过**包含有GLSL源码的资源文件**添加：

        `QOpenGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::源码类型, ":/路径");`

        **顶点着色器的文件名后缀应该是 `.vert`，片段着色器为 `.frag`**

        *可以通过右键文件，拷贝文件路径*

        <img src="doc/pic/README/image-20220905200249305.png" alt="image-20220905200249305" style="zoom: 67%;" />

        

- **需要传入的第一个参数类型可以为：**

    - `QOpenGLShader::Vertex` - 顶点着色器
    - `QOpenGLShader::Fragment` - 片段着色器

```c++
#include <QOpenGLShaderProgram>

class FoxOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_X_X_Core
{
  
  ...
    
    
private:
  ...
  QOpenGLShaderProgram shader_program_;
}

/**
* 可以通过 Qt 提供的 `QOpenGLShaderProgram` 对象进行 GLSL 源码（着色器）的编译和链接，可以使代码量大幅度减少
*/
void FoxOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  // 【重点】初始化OpenGL函数，将 Qt 里的函数指针指向显卡的函数（头文件 QOpenGLFunctions_X_X_Core）


    // ===================== VAO | VBO =====================
    ...

    // ===================== EBO =====================
    ...


    bool success;
    // ===================== 顶点着色器 =====================
    this->shader_program_.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);

    // ===================== 片段着色器 =====================
    this->shader_program_.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    // ===================== 链接着色器 =====================
    success = this->shader_program_.link();

    if (!success)
    {
        qDebug << "ERROR: " << this->shader_program_.log();
    }
		
  ...
}
```



**通过 Qt 资源文件的形式将 GLSL 源码传入：**

![image-20220905184211935](doc/pic/README/image-20220905184211935.png)

![image-20220905184318358](doc/pic/README/image-20220905184318358.png)

![image-20220905184413706](doc/pic/README/image-20220905184413706.png)

![image-20220905184728533](doc/pic/README/image-20220905184728533.png)

![image-20220905184859891](doc/pic/README/image-20220905184859891.png)



# GLSL

**GLSL** - Open**GL** Shading **L**anguage 的缩写，是一种类 C 语言，一般拥有以下结构：

```glsl
#version VERSION_NUMBER  // 版本号
in TYPE IN_VARIABLE_NAME;  // `in` 代表在流水线上的来自其他模块的输入
in TYPE IN_VARIABLE_NAME;
out TYPE OUT_VARIABLE_NAME;  // `out` 代表本模块的输出，要输入到下一个模块的东西
uniform TYPE UNIFORM_NAME;  // 

void main() {
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  OUR_VARIABLE_NAME = WEIRD_STUFF_WE_PROCESSED;
}
```

当我们特别谈论到顶点着色器的时候，每个输入变量也叫**顶点属性(Vertex Attribute)**，也就是我们写在 GLSL `layout (location = ?)`那里的。我们能声明的顶点属性是有上限的，它一般由硬件来决定。OpenGL确保至少有16个包含4分量的顶点属性可用，但是有些硬件或许允许更多的顶点属性，你可以查询GL_MAX_VERTEX_ATTRIBS来获取具体的上限：

```
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
```

通常情况下它至少会返回16个，大部分情况下是够用了。



## 数据类型

GLSL中包含C等其它语言大部分的默认基础数据类型：`int`、`float`、`double`、`uint`和`bool`。GLSL也有两种容器类型，它们会在这个教程中使用很多，分别是向量(Vector)和矩阵(Matrix)，其中矩阵我们会在之后的教程里再讨论。

### 向量

GLSL中的向量是一个可以包含有2、3或者4个分量的容器，分量的类型可以是前面默认基础类型的任意一个。它们可以是下面的形式（`n`代表分量的数量）：

| 类型    | 含义                            |
| :------ | :------------------------------ |
| `vecn`  | 包含`n`个float分量的默认向量    |
| `bvecn` | 包含`n`个bool分量的向量         |
| `ivecn` | 包含`n`个int分量的向量          |
| `uvecn` | 包含`n`个unsigned int分量的向量 |
| `dvecn` | 包含`n`个double分量的向量       |

**大多数时候我们使用`vecn`，因为float足够满足大多数要求了。**

一个向量的分量可以通过`vec.x`这种方式获取，这里`x`是指这个向量的第一个分量。你可以分别使用`.x`、`.y`、`.z`和`.w`来获取它们的第1、2、3、4个分量。GLSL也允许你对颜色使用`rgba`，或是对纹理坐标使用`stpq`访问相同的分量。

向量这一数据类型也允许一些有趣而灵活的分量选择方式，叫做**重组(Swizzling)**。重组允许这样的语法：

```glsl
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

你可以使用上面4个字母任意组合来创建一个和原来向量一样长的（同类型）新向量，只要原来向量有那些分量即可；然而，你不允许在一个`vec2`向量中去获取`.z`元素。我们也可以把一个向量作为一个参数传给不同的向量构造函数，以减少需求参数的数量：

```glsl
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```





## 输入与输出

### in & out

**因为 OpenGL 像是一个流水线，`in` 就是流水线上流进来的变量，`out` 就是流出的变量**

虽然着色器是各自独立的小程序，但是它们都是一个整体的一部分，出于这样的原因，我们希望每个着色器都有输入和输出，这样才能进行数据交流和传递。GLSL定义了`in`和`out`关键字专门来实现这个目的。**每个着色器使用这两个关键字设定输入和输出，只要一个输出变量与下一个着色器阶段的输入匹配，它就会传递下去。但在顶点和片段着色器中会有点不同。**

所以，如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。**当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在==链接程序对象时完成的==）**。为了展示这是如何工作的，我们会稍微改动一下之前教程里的那个着色器，让顶点着色器为片段着色器决定颜色。



**顶点着色器**

```glsl
#version 330 core
layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0

out vec4 vertexColor; // 为片段着色器指定一个颜色输出

void main()
{
    gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色
}
```

**片段着色器**

```glsl
#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

void main()
{
    FragColor = vertexColor;
}
```

你可以看到我们在顶点着色器中声明了一个vertexColor变量作为`vec4`输出，并在片段着色器中声明了一个类似的vertexColor。由于它们名字相同且类型相同，片段着色器中的vertexColor就和顶点着色器中的vertexColor链接了。由于我们在顶点着色器中将颜色设置为深红色，最终的片段也是深红色的。下面的图片展示了输出结果：

![img](https://learnopengl-cn.github.io/img/01/05/shaders.png)



### layout

**顶点着色器应该接收的是一种特殊形式的输入，否则就会效率低下。**顶点着色器的输入特殊在，它从顶点数据中直接接收输入。**数据是来自于 CPU 端**



为了定义顶点数据该如何管理，我们使用`location`这一元数据指定输入变量，这样我们才可以在CPU上配置顶点属性。我们已经在前面的教程看过这个了，`layout (location = 0)`。顶点着色器需要为它的输入提供一个额外的`layout`标识，这样我们才能把它**链接到顶点数据**。

你也可以忽略`layout (location = 0)`标识符，通过在OpenGL代码中使用`glGetAttribLocation`查询属性位置值(Location) 再使用 `glBindAttribLocation()` 设置位置值，但是我更喜欢在着色器中设置它们，这样会更容易理解而且节省你（和OpenGL）的工作量。



另一个例外是**片段着色器，它需要一个`vec4`颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。**如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。

> 如果使用 QOpenGLShaderProgram，则在**绑定着色器原码之后**可以通过其提供的：
>
> - `QOpenGLShaderProgram.attributeLocation("aPos");` 方法手动找到顶点属性的位置
> - 如果顶点着色器里没写 `layout`，则可以用`QOpenGLShaderProgram.bindAttributeLocation("aPos", LOCATION_NUMBER);` 手动指定顶点属性位置
>
> **但是注意，使用 QOpenGLShaderProgram 前最好先 `.bind()` 一下，避免出错**

```c++
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

#if 0
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
#endif

#if 1
    /* 当我们在顶点着色器中没有写 layout 时，也可以在此处代码根据名字手动指定某个顶点属性的位置 */
    this->shader_program_.bind();
    GLint aPosLocation = 2;
    this->shader_program_.bindAttributeLocation("aPos", aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosLocation);
#endif
```



## uniform

**Uniform 是一种从CPU中的应用向GPU中的着色器发送数据的方式**，但uniform和顶点属性有些不同。

- 首先，**uniform是全局的(Global)**。全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，而且**它可以被着色器程序的任意着色器在任意阶段访问**。
- 第二，无论你把uniform值设置成什么，uniform 会一直保存它们的数据，直到它们被重置或更新。

我们可以在一个着色器中添加`uniform`关键字至类型和变量名前来声明一个GLSL的uniform。从此处开始我们就可以在着色器中使用新声明的uniform了。我们来看看这次是否能通过uniform设置三角形的颜色：

```glsl
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

void main()
{
    FragColor = ourColor;
}
```

我们在片段着色器中声明了一个uniform `vec4`的ourColor，并把片段着色器的输出颜色设置为uniform值的内容。因为uniform是全局变量，我们可以在任何着色器中定义它们，而无需通过顶点着色器作为中介。顶点着色器中不需要这个uniform，所以我们不用在那里定义它。

*如果你声明了一个uniform却在GLSL代码中没用过，编译器会静默移除这个变量，导致最后编译出的版本中并不会包含它，这可能导致几个非常麻烦的错误，记住这点！*

这个uniform现在还是空的；我们还没有给它添加任何数据，所以下面我们就做这件事。我们**首先需要找到着色器中uniform属性的索引/位置值。当我们得到uniform的索引/位置值后，我们就可以更新它的值了**。这次我们不去给像素传递单独一个颜色，而是让它随着时间改变颜色：

```c++
int timeValue = QTime::currentTime().second();
float greenValue = (sin(timeValue)/2.0f) + 0.5f;
shaderProgram.setUniformValue("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
```

以上部分其实是 QShaderProgram 进行了封装，**源代码**应该为：

```c++
int vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 0.0f);
```



OpenGL在其核心是一个C库， 所以它不支持类型重载，在函数参数类型不同的时候就要为其定义新的函数； glUniform是一个典型例子。 这个函数有一个特定的后缀，标识设定的uniform的类型。可能的后缀有：

![image-20220912113006689](doc/pic/README/image-20220912113006689.png)



## 更多属性

在前面的教程中，我们了解了如何填充VBO、配置顶点属性指针以及如何把它们都储存到一个VAO里。这次，我们同样打算把颜色数据加进顶点数据中。我们将把颜色数据添加为3个float值至vertices数组。我们将把三角形的三个角分别指定为红色、绿色和蓝色：

```c++
float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
```

由于现在有更多的数据要发送到顶点着色器，我们有必要去调整一下顶点着色器，使它能够接收颜色值作为一个顶点属性输入。需要注意的是我们用`layout`标识符来把aColor属性的位置值设置为1：

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}
```

由于我们不再使用uniform来传递片段的颜色了，现在使用`ourColor`输出变量，我们必须再修改一下片段着色器：

```glsl
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
```

因为我们添加了另一个顶点属性，并且更新了VBO的内存，我们就必须重新配置顶点属性指针。更新后的VBO内存中的数据现在看起来像这样：

![img](https://learnopengl-cn.github.io/img/01/05/vertex_attribute_pointer_interleaved.png)

知道了现在使用的布局，我们就可以使用glVertexAttribPointer函数更新顶点格式，

```c++
// 位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 颜色属性
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);
```

glVertexAttribPointer函数的前几个参数比较明了。这次我们配置属性位置值为1的顶点属性。颜色值有3个float那么大，我们不去标准化这些值。

由于我们现在有了两个顶点属性，我们不得不重新计算**步长**值。为获得数据队列中下一个属性值（比如位置向量的下个`x`分量）我们必须向右移动6个float，其中3个是位置值，另外3个是颜色值。这使我们的步长值为6乘以float的字节数（=24字节）。
同样，这次我们必须指定一个偏移量。对于每个顶点来说，位置顶点属性在前，所以它的偏移量是0。颜色属性紧随位置数据之后，所以偏移量就是`3 * sizeof(float)`，用字节来计算就是12字节。

运行程序你应该会看到如下结果：

![img](https://learnopengl-cn.github.io/img/01/05/shaders3.png)

如果你在哪卡住了，可以在[这里](https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/3.2.shaders_interpolation/shaders_interpolation.cpp)查看源码。

这个图片可能不是你所期望的那种，因为我们只提供了3个颜色，而不是我们现在看到的大调色板。这是在片段着色器中进行的所谓**片段插值(Fragment Interpolation)**的结果。当渲染一个三角形时，**光栅化(Rasterization)**阶段通常会造成比原指定顶点更多的片段。光栅会根据每个片段在三角形形状上所处相对位置决定这些片段的位置。
基于这些位置，它会**插值(Interpolate)**所有片段着色器的输入变量。比如说，我们有一个线段，上面的端点是绿色的，下面的端点是蓝色的。如果一个片段着色器在线段的70%的位置运行，它的颜色输入属性就会是一个绿色和蓝色的线性结合；更精确地说就是30%蓝 + 70%绿。

这正是在这个三角形中发生了什么。我们有3个顶点，和相应的3个颜色，从这个三角形的像素来看它可能包含50000左右的片段，片段着色器为这些像素进行插值颜色。如果你仔细看这些颜色就应该能明白了：红首先变成到紫再变为蓝色。片段插值会被应用到片段着色器的所有输入属性上。



# 纹理

纹理 - Texel

艺术家和程序员更喜欢使用**纹理(Texture)**。纹理是一个2D图片（甚至也有1D和3D的纹理），它可以用来添加物体的细节；你可以想象纹理是一张绘有砖块的纸，无缝折叠贴合到你的3D的房子上，这样你的房子看起来就像有砖墙外表了。因为我们可以在一张图片上插入非常多的细节，这样就可以让物体非常精细而不用指定额外的顶点。

除了图像以外，纹理也可以被用来储存大量的数据，这些数据可以发送到着色器上，但是这不是我们现在的主题。

下面你会看到之前教程的那个三角形贴上了一张[砖墙](https://learnopengl-cn.github.io/img/01/06/wall.jpg)图片。

![img](https://learnopengl-cn.github.io/img/01/06/textures.png)

为了能够把纹理**映射(Map)**到三角形上，我们需要指定三角形的每个顶点各自对应纹理的哪个部分。这样每个顶点就会关联着一个**纹理坐标(Texture Coordinate)**，用来标明该从纹理图像的哪个部分采样（译注：采集片段颜色）。之后在图形的其它片段上进行片段**插值(Fragment Interpolation)**。

纹理坐标在x和y轴上，范围为0到1之间（注意我们使用的是2D纹理图像）。使用纹理坐标获取纹理颜色叫做**采样(Sampling)**。纹理坐标起始于(0, 0)，也就是纹理图片的左下角，终始于(1, 1)，即纹理图片的右上角。下面的图片展示了我们是如何把纹理坐标映射到三角形上的。

![img](https://learnopengl-cn.github.io/img/01/06/tex_coords.png)

我们为三角形指定了3个纹理坐标点。如上图所示，我们希望三角形的左下角对应纹理的左下角，因此我们把三角形左下角顶点的纹理坐标设置为(0, 0)；三角形的上顶点对应于图片的上中位置所以我们把它的纹理坐标设置为(0.5, 1.0)；同理右下方的顶点设置为(1, 0)。我们只要给顶点着色器传递这三个纹理坐标就行了，接下来它们会被传片段着色器中，它会为每个片段进行纹理坐标的插值。

纹理坐标看起来就像这样：

```
float texCoords[] = {
    0.0f, 0.0f, // 左下角
    1.0f, 0.0f, // 右下角
    0.5f, 1.0f // 上中
};
```

对纹理采样的解释非常宽松，它可以采用几种不同的插值方式。所以我们需要自己告诉OpenGL该怎样对纹理**采样**。
