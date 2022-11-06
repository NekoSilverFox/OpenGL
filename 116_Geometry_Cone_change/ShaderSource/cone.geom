#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 131) out;


out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;


uniform float time;
uniform float del_h;  // 高度偏移量
uniform float del_b;  // 底部圆心偏移量
uniform float add_point_step_angle;
uniform int num_add_points;

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
    vec3 new_normal;

    pos_1 = changePos(gl_in[0].gl_Position);
    pos_2 = changePos(gl_in[1].gl_Position);

    /* 圆心坐标 C(c_x, c_y, c_z) */
    vec3    c   = vec3(pos_1.x, 0.0f, pos_1.z);
    vec4    last_pos;
    vec4    add_pos;
    add_pos = vec4(c.x + (pos_2.x - c.x)*cos(radians(add_point_step_angle)) - (pos_2.y - c.y)*sin(radians(add_point_step_angle)),
                   pos_2.y,
                   c.y + (pos_2.x - c.x)*sin(radians(add_point_step_angle)) - (pos_2.y - c.y)*cos(radians(add_point_step_angle)),
                   pos_2.w);
    new_normal = GetNormal(pos_1, pos_2, add_pos);
    sendPoint2FragShader(pos_1, new_normal);
    sendPoint2FragShader(pos_2, new_normal);
    sendPoint2FragShader(add_pos, new_normal);
    last_pos = add_pos;

    for (int i = 0; i < num_add_points - 1; i++)
    {
    add_pos = vec4(c.x + (pos_2.x - c.x)*cos(radians(add_point_step_angle * (i + 2))) - (pos_2.y - c.y)*sin(radians(add_point_step_angle * (i + 2))),
                   pos_2.y,
                   c.y + (pos_2.x - c.x)*sin(radians(add_point_step_angle * (i + 2))) - (pos_2.y - c.y)*cos(radians(add_point_step_angle * (i + 2))),
                   pos_2.w);
        new_normal = GetNormal(pos_1, last_pos, add_pos);
        sendPoint2FragShader(pos_1, new_normal);
        sendPoint2FragShader(last_pos, new_normal);
        sendPoint2FragShader(add_pos, new_normal);
        last_pos = add_pos;
    }

    pos_3 = changePos(gl_in[2].gl_Position);
    new_normal = GetNormal(pos_1, add_pos, pos_3);

    sendPoint2FragShader(pos_1, new_normal);
    sendPoint2FragShader(add_pos, new_normal);
    sendPoint2FragShader(pos_3, new_normal);
    EndPrimitive();
}
