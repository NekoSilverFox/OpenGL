#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;


/* 获取当前片段（三角形）的法线值 */
vec3 getNormal(vec4 pos_1, vec4 pos_2, vec4 pos_3)
{
    vec3 a = pos_1.xyz - pos_2.xyz;
    vec3 b = pos_3.xyz - pos_2.xyz;
    return cross(a, b);
}

/**
  * 将点和其法线发送到片段着色器
  */
void sendPoint2FragShader(vec4 pos, vec3 normal)
{
    gl_Position         = mat_projection * mat_view * mat_model * pos;   // 【注意最后相乘】乘以矩阵实现旋转和位移的效果，注意矩阵运算方向
    res_g_axis_normal   = mat3(transpose  (inverse(mat_model))) * normal;
    res_g_axis_fragPos  = vec3(mat_model * pos);
    EmitVertex();
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ main @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void main() {

    vec4    pos_1    = gl_in[0].gl_Position;
    vec4    pos_2    = gl_in[1].gl_Position;
    vec4    pos_3    = gl_in[2].gl_Position;
    vec3    normal   = getNormal(pos_1, pos_2, pos_3);

    sendPoint2FragShader(pos_1, normal);
    sendPoint2FragShader(pos_2, normal);
    sendPoint2FragShader(pos_3, normal);

    EndPrimitive();
}
