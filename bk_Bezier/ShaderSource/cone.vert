#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;   // 法线值


out vec3 g_axis_normal;
out vec3 g_axis_fragPos;
out vec2 texCoord;


uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

void main()
{
    // 由顶点着色器指定和发送控制点(这里硬编码写死)
    const vec4 vectices[] = vec4[] {
        vec4(-1.0, 0.5, -1.0, 1.0),
        vec4(-0.5, 0.5, -1.0, 1.0),
        vec4( 0.5, 0.5, -1.0, 1.0),
        vec4( 1.0, 0.5, -1.0, 1.0),

        vec4(-1.0, 0.0, -0.5, 1.0),
        vec4(-0.5, 0.0, -0.5, 1.0),
        vec4( 0.5, 0.0, -0.5, 1.0),
        vec4( 1.0, 0.0, -0.5, 1.0),

        vec4(-1.0, 0.0, 0.5, 1.0),
        vec4(-0.5, 0.0, 0.5, 1.0),
        vec4( 0.5, 0.0, 0.5, 1.0),
        vec4( 1.0, 0.0, 0.5, 1.0),

        vec4(-1.0, -0.5, 1.0, 1.0),
        vec4(-0.5,  0.3, 1.0, 1.0),
        vec4( 0.5,  0.3, 1.0, 1.0),
        vec4( 1.0,  0.3, 1.0, 1.0)
    }
    // 为当前顶点计算合适的纹理坐标，从[-1...+1]转换到[0...1]
    texCoord = vec2((vertices[gl_VertexID].x + 1.0) / 2.0,
                    (vertices[gl_VertexID].z + 1.0) / 2.0);


    gl_Position = mat_projection * mat_view * mat_model * vec4(aPos, 1.0);  // 乘以矩阵实现旋转和位移的效果，注意矩阵运算方向

    g_axis_normal = mat3(transpose(inverse(mat_model))) * aNormal;  // 将模型[法线]的坐标转换到世界坐标
    g_axis_fragPos = vec3(mat_model * vec4(aPos, 1.0));             // 将模型[片段]的坐标转换到世界坐标
}
