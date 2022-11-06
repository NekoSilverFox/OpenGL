#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;


uniform float time;
uniform float del_h;  // 高度偏移量
uniform float del_b;  // 底部圆心偏移量

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;


vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 100.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}


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


void main() {

    vec4 pos_1 = changePos(gl_in[0].gl_Position);
    vec4 pos_2 = changePos(gl_in[1].gl_Position);
    vec4 pos_3 = changePos(gl_in[2].gl_Position);

    vec3 new_normal = GetNormal(pos_1, pos_2, pos_3);


    gl_Position = mat_projection * mat_view * mat_model * pos_1;   // 【注意最后相乘】乘以矩阵实现旋转和位移的效果，注意矩阵运算方向
    res_g_axis_normal   = mat3(transpose(inverse(mat_model))) * new_normal;
    res_g_axis_fragPos  = vec3(mat_model * pos_1);
    EmitVertex();


    gl_Position = mat_projection * mat_view * mat_model * pos_2;
    res_g_axis_normal   = mat3(transpose(inverse(mat_model))) * new_normal;
    res_g_axis_fragPos  = vec3(mat_model * pos_2);
    EmitVertex();


    gl_Position = mat_projection * mat_view * mat_model * pos_3;
    res_g_axis_normal   = mat3(transpose(inverse(mat_model))) * new_normal;
    res_g_axis_fragPos  = vec3(mat_model * pos_3);
    EmitVertex();


    EndPrimitive();
}
