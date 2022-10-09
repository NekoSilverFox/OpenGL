#version 330 core
out vec4 FragColor;

uniform float val_alpha;

void main()
{
    FragColor = mix(vec4(1.0f, 0.0f, 0.0f, 1.0f),
                    vec4(0.0f, 1.0f, 0.0f, 1.0f),
                    val_alpha);
}
