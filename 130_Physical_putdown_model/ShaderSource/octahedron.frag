#version 330 core
out vec4 FragColor;

in vec3 res_g_axis_normal;
in vec3 res_g_axis_fragPos;

/* 材质颜色 */
struct Material {
    vec3 ambient;		// 环境光颜色
    vec3 diffuse;		// 漫反射颜色
    vec3 specular;      // 镜面高光颜色
    float shininess;    // 镜面光高光的散射/半径
};

/* 光源颜色 */
struct Light {
    vec3 ambient;		// 环境光颜色
    vec3 diffuse;		// 漫反射颜色
    vec3 specular;      // 镜面光颜色
    float shininess;    // 镜面光高光的散射/半径
};

uniform Material material;
uniform Light light;

uniform vec3 light_pos;   // 光源位置
uniform vec3 view_pos;

void main()
{
    /* 环境光 */
    vec3    res_ambient  = light.ambient * material.ambient;  // 环境光及强度

    /* 漫反射光 - diffuse */
    vec3    normal       = normalize(res_g_axis_normal);  // 标准化法线
    vec3    light_dir    = normalize(light_pos - res_g_axis_fragPos); // 片段指向光源的方向向量
    float   diff         = max(dot(normal, light_dir), 0.0);  // 光源对当前片段实际的漫反射影响系数
    vec3    res_diff     = light.diffuse * material.diffuse * diff;

    /* 反射光 - specular */
    vec3    view_dir     = normalize(view_pos - res_g_axis_fragPos);
    vec3    reflect_dir  = reflect(-light_dir, normal);
    float   spec         = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    vec3    res_specular = light.specular * material.specular * spec;


    vec3 result = res_ambient + res_diff + res_specular;  // 叠加物体颜色
    FragColor = vec4(result, 1.0);
}
