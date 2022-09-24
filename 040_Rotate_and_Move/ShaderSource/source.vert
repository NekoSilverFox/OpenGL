#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexel; // 存储2D纹理坐标s、t

out vec3 ourColor; // 向片段着色器输出一个颜色
out vec2 ourTexel; // 向片段着色器输出2D纹理坐标s、t

uniform mat4 matrix;

void main()
{
    gl_Position = matrix * vec4(aPos, 1.0);  // 乘以矩阵实现旋转和位移的效果
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
    ourTexel = vec2(aTexel.s * -1, aTexel.t);
}
