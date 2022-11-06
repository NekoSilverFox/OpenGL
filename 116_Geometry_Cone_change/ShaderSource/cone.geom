#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;


uniform float time;
uniform float del_h;  // 高度偏移量
uniform float del_b;  // 底部圆心偏移量
uniform int drop_pos_mod;

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;


vec4 changePos(vec4 position)
{
    return vec4(position.x * del_b,
                position.y * del_h,
                position.z * del_b,
                position.w);
}

/* 获取当前片段（三角形）的法线值 */
vec3 GetNormal(vec4 pos_1, vec4 pos_2, vec4 pos_3)
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
    gl_Position = mat_projection * mat_view * mat_model * pos;   // 【注意最后相乘】乘以矩阵实现旋转和位移的效果，注意矩阵运算方向
    res_g_axis_normal   = mat3(transpose(inverse(mat_model))) * normal;
    res_g_axis_fragPos  = vec3(mat_model * pos);
    EmitVertex();
}

void main() {
    vec4 pos_1;
    vec4 pos_2;
    vec4 pos_3;

    /* 圆心坐标 C(c_x, c_y, c_z) */
//    vec3 c = vec3(gl_in[0].gl_Position.x, 0.0f, gl_in[0].gl_Position.z)
//    float r = sqrt(pow(pos_2.x, 2), pow(pos_2.z, 2));
//    vec3 add_pos = vec3(c.x - r*sin(45), pos_2.y, c.z - r*cos(45))

//    if (mod(gl_PrimitiveIDIn, drop_pos_mod) != 0)
//    {
//        pos_1 = changePos(gl_in[0].gl_Position);
//        pos_2 = changePos(gl_in[1].gl_Position);
//        pos_3 = changePos(gl_in[2].gl_Position);
//    }
//    else
//    {
//        pos_1 = changePos(gl_in[0].gl_Position);
//        pos_2 = changePos(gl_in[1].gl_Position);
//        pos_3 = changePos(gl_in[2].gl_Position);
//    }

    pos_1 = changePos(gl_in[0].gl_Position);
    pos_2 = changePos(gl_in[1].gl_Position);
    pos_3 = changePos(gl_in[2].gl_Position);


    vec3 new_normal = GetNormal(pos_1, pos_2, pos_3);


    sendPoint2FragShader(pos_1, new_normal);
    sendPoint2FragShader(pos_2, new_normal);
    sendPoint2FragShader(pos_3, new_normal);
    EndPrimitive();
}
