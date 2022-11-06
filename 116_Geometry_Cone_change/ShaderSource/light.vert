#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

void main()
{
    gl_Position = mat_projection * mat_view * mat_model * vec4(aPos, 1.0);  // 乘以矩阵实现旋转和位移的效果，注意矩阵运算方向
}
