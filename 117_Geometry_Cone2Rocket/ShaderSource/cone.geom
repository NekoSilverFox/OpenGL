#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 170) out;

out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;
out float color_id;

uniform float time;
uniform float del_h;  // 高度偏移量
uniform float del_b;  // 底部圆心偏移量
uniform float add_point_step_angle;
uniform int num_add_points;
uniform float r;
uniform float booster_R;  // 助推器长度（宽度）

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;


uniform bool is_draw_cylinder;
uniform bool is_draw_booster;
uniform float heigh_cylinder;

bool is_al_draw_booster = false;




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
    gl_Position         = mat_projection * mat_view * mat_model * pos;   // 【注意最后相乘】乘以矩阵实现旋转和位移的效果，注意矩阵运算方向
    res_g_axis_normal   = mat3(transpose  (inverse(mat_model))) * normal;
    res_g_axis_fragPos  = vec3(mat_model * pos);
    EmitVertex();
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ main @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void main() {

    color_id = 0.0f;


    vec4    pos_1    = changePos(gl_in[0].gl_Position);
    vec4    pos_2    = changePos(gl_in[1].gl_Position);
    vec3    c        = vec3(pos_1.x, 0.0f, pos_1.z);  // 圆锥底部圆心坐标 C(c_x, c_y, c_z)
    vec4    last_pos = pos_2;
    /* 构建新片段 */
    for (int i = 1; i <= num_add_points; i++)
    {
        /* 细分片段（变得圆滑）*/
        vec4 add_pos = vec4(c.x + (pos_2.x - c.x)*cos(radians(add_point_step_angle * i)) - (pos_2.y - c.y)*sin(radians(add_point_step_angle * i)),
                            pos_2.y,
                            c.y + (pos_2.x - c.x)*sin(radians(add_point_step_angle * i)) - (pos_2.y - c.y)*cos(radians(add_point_step_angle * i)),
                            pos_2.w);

        /*          底面不拉伸               底面拉伸但，不是底部（面）的点 */
        if ((is_draw_cylinder == false) || (pos_1.y != last_pos.y))
        {
            vec3 new_normal = GetNormal(pos_1, last_pos, add_pos);
            sendPoint2FragShader(pos_1,    new_normal);
            sendPoint2FragShader(last_pos, new_normal);
            sendPoint2FragShader(add_pos,  new_normal);
            EndPrimitive();
        }

        /* 如果是底面的点，增加底部的点（向下拉伸） */
        else
        {
            vec4 add_cylinder_pos = vec4(last_pos.x, last_pos.y - heigh_cylinder, last_pos.z, last_pos.w);
            vec3 new_normal = GetNormal(last_pos, add_cylinder_pos,  add_pos);
            sendPoint2FragShader(last_pos,          new_normal);
            sendPoint2FragShader(add_cylinder_pos,  new_normal);
            sendPoint2FragShader(add_pos,           new_normal);
            EndPrimitive();


            vec4 add_cylinder_pos_2 = vec4(add_pos.x, add_pos.y - heigh_cylinder, add_pos.z, add_pos.w);
            sendPoint2FragShader(add_pos,           new_normal);
            sendPoint2FragShader(add_cylinder_pos,  new_normal);
            sendPoint2FragShader(add_cylinder_pos_2,new_normal);
            EndPrimitive();
        }
        last_pos = add_pos;
    }


    /*  如果圆度足够，就增加助推器 */
    if (is_draw_booster && (pos_1.y == last_pos.y) && (is_draw_cylinder == true))
    {
          is_al_draw_booster = true;
          vec4 booster_top     ;
          vec4 booster_bottom_1;
          vec4 booster_bottom_2;
          vec4 booster_bottom_3;
          vec3 normal;



        for (float angle_offset = 0.0f; angle_offset <= 360.0f; angle_offset += 90.0f)
        {
            float lr_offset = 40.0f;
            booster_top      = vec4(r*sin(radians(45.0f+angle_offset)),                      c.y - heigh_cylinder/3,
                                    r*cos(radians(45.0f+angle_offset)),                      1.0f);

            booster_bottom_1 = vec4(r*sin(radians(45.0f-lr_offset+angle_offset)),            c.y - heigh_cylinder,
                                    r*cos(radians(45.0f-lr_offset+angle_offset)),            1.0f);

            booster_bottom_2 = vec4((r+booster_R)*sin(radians(45.0f+angle_offset)),          c.y - heigh_cylinder,
                                    (r+booster_R)*cos(radians(45.0f+angle_offset)),          1.0f);

            booster_bottom_3 = vec4(r*sin(radians(45.0f+lr_offset+angle_offset)),            c.y - heigh_cylinder,
                                    r*cos(radians(45.0f+lr_offset+angle_offset)),            1.0f);



            normal = -GetNormal(booster_top, booster_bottom_1, booster_bottom_2);
            sendPoint2FragShader(booster_top,           normal);
            sendPoint2FragShader(booster_bottom_1,      normal);
            sendPoint2FragShader(booster_bottom_2,      normal);
            EndPrimitive();

            normal = -GetNormal(booster_top, booster_bottom_2, booster_bottom_3);
            sendPoint2FragShader(booster_top,           normal);
            sendPoint2FragShader(booster_bottom_3,      normal);
            sendPoint2FragShader(booster_bottom_2,      normal);
            EndPrimitive();

        }
    }

//    EndPrimitive();

}
