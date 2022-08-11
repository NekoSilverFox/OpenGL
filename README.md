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





















