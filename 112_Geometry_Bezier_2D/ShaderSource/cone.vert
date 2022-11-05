#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;   // 法线值


out VS_OUT {
    vec3 g_axis_normal;
    vec3 g_axis_fragPos;
    int id;
} vs_out;


uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

void main()
{
    gl_Position = mat_projection * mat_view * mat_model * vec4(aPos, 1.0);  // 乘以矩阵实现旋转和位移的效果，注意矩阵运算方向

    vs_out.g_axis_normal = mat3(transpose(inverse(mat_model))) * aNormal;  // 将模型[法线]的坐标转换到世界坐标
    vs_out.g_axis_fragPos = vec3(mat_model * vec4(aPos, 1.0));             // 将模型[片段]的坐标转换到世界坐标
    vs_out.id = gl_VertexID;
}
