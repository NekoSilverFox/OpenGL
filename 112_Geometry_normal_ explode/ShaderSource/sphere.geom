#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 g_axis_normal;
    vec3 g_axis_fragPos;
} gs_in[];  // 注意，这里是对于图元来说的。也就是三角形的各个点


out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;



uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 100.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

void main() {
    gl_Position = explode(gl_in[0].gl_Position, gs_in[0].g_axis_normal);
    res_g_axis_normal   = gs_in[0].g_axis_normal;
    res_g_axis_fragPos  = gs_in[0].g_axis_fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, gs_in[1].g_axis_normal);
    res_g_axis_normal   = gs_in[1].g_axis_normal;
    res_g_axis_fragPos  = gs_in[1].g_axis_fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, gs_in[2].g_axis_normal);
    res_g_axis_normal   = gs_in[2].g_axis_normal;
    res_g_axis_fragPos  = gs_in[2].g_axis_fragPos;
    EmitVertex();

    EndPrimitive();
}
