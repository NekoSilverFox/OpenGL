#version 330 core
out vec4 FragColor;

//in vec2 ourTexel;

// （纹理单元）其实自己不用写，默认是有的，用于传入纹理数据；【sampler是采样器】
//uniform sampler2D texture0;
//uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform float val_alpha;

void main()
{
//    FragColor = mix(texture(texture0, ourTexel),
//                    texture(texture1, ourTexel), val_alpha);
    FragColor = mix(vec4(1.0f, 0.0f, 0.0f, 1.0f),
                    vec4(0.0f, 1.0f, 0.0f, 1.0f),
                    val_alpha);
}
