#version 330 core
out vec4 FragColor;


in vec3 g_axis_normal;
in vec3 g_axis_fragPos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;   // 光源位置
uniform vec3 view_pos;

void main()
{
    /* 环境光 */
    float ambient_strength = 0.2; // 环境光强度因子，表示物体最终只能反射 0.1
    vec3 res_ambient = ambient_strength * light_color;  // 环境光及强度

    /* 漫反射光 - diffuse */
    vec3 normal     = normalize(g_axis_normal);  // 标准化法线
    vec3 light_dir  = normalize(light_pos - g_axis_fragPos); // 片段指向光源的方向向量
    float diff      = max(dot(normal, light_dir), 0.0);  // 光源对当前片段实际的漫反射影响系数
    vec3 res_diff   = diff * light_color;

    /* 反射光 - specular */
    float specular_strength = 0.5;
    vec3 view_dir = normalize(view_pos - g_axis_fragPos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 256);
    vec3 res_specular = specular_strength * spec * light_color;

    vec3 result = (res_ambient + res_diff + res_specular) * object_color;  // 叠加物体颜色

    FragColor = vec4(result, 1.0);
}
