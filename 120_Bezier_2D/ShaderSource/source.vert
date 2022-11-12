#version 330 core
layout (location = 0) in vec3 aPos;


out VS_OUT{
    int id;
} vs_out;


void main()
{
   gl_Position = vec4(aPos.x + 0.2f, aPos.y + 0.2f, aPos.z, 1.0f);

   vs_out.id = gl_VertexID;
}
