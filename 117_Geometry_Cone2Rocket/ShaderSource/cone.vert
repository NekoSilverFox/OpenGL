#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0

void main()
{
    gl_PointSize = 3.0f;
    gl_Position = vec4(aPos, 1.0);
}
