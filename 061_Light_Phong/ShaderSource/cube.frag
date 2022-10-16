#version 330 core
out vec4 FragColor;


in vec3 Normal;
in vec3 FragPos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;

void main()
{
    /* 环境光 */
    float ambient_strength = 0.1; // 环境光强度因子，表示物体最终只能反射 0.1
    vec3 ambient = ambient_strength * light_color;  // 环境光及强度

    /* 漫反射 - diffuse */
    vec3 normal = normalize(Normal);
    vec3 light_dir = normalize(light_pos - FragPos); // 片段指向光源的方向向量
    float diff = max(dot(normal, light_dir), 0.0);  // 光源对当前片段实际的漫反射影响
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient + diffuse) * object_color;  // 叠加物体颜色

    FragColor = vec4(result, 1.0);
}
