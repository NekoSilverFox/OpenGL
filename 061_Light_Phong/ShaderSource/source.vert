#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
//layout (location = 1) in vec2 aTexel; // 存储2D纹理坐标s、t

//out vec2 ourTexel; // 向片段着色器输出2D纹理坐标s、t

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

void main()
{
    gl_Position = mat_projection * mat_view * mat_model * vec4(aPos, 1.0);  // 乘以矩阵实现旋转和位移的效果，注意矩阵运算方向

   // ourTexel = vec2(aTexel.s, aTexel.t);
}
