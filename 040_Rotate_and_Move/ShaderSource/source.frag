#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexel;

// （纹理单元）其实自己不用写，默认是有的，用于传入纹理数据；【sampler是采样器】
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float val_alpha;

void main()
{
//    FragColor = vec4(ourColor, 1.0);
//    FragColor = texture(texture0, ourTexel) * vec4(0.5, 0.7, 0.0, 0.2);  // 可以使用乘法改变颜色
//    FragColor = mix(texture(texture0, ourTexel),
//                    texture(texture1, ourTexel),
//                    0.5);
//    FragColor = texture(texture1, ourTexel);
    FragColor = mix(texture(texture0, ourTexel),
                    texture(texture1, ourTexel), val_alpha);
}
