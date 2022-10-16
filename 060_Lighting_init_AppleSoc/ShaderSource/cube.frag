#version 330 core
out vec4 FragColor;

uniform vec3 object_color;  // 物体颜色
uniform vec3 light_color;   // 环境光颜色

void main()
{
    FragColor = vec4(object_color * light_color, 1.0f);
}
