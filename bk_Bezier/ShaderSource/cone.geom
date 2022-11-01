#version 330 core
layout(quads, equal_spacing, ccw) in;
uniform mat4 mvp_matrix;
in vec2 texCoord_TCSout[];// 控制点纹理坐标以数组形式传进来
out vec2 texCoord_TESout;// 每个网格顶点纹理坐标以标量形式传出
void main() {
    vec3 p00 = (gl_in[0].gl_Position).xyz;
    vec3 p10 = (gl_in[1].gl_Position).xyz;
    vec3 p20 = (gl_in[2].gl_Position).xyz;
    vec3 p30 = (gl_in[3].gl_Position).xyz;
    vec3 p01 = (gl_in[4].gl_Position).xyz;
    vec3 p11 = (gl_in[5].gl_Position).xyz;
    vec3 p21 = (gl_in[6].gl_Position).xyz;
    vec3 p31 = (gl_in[7].gl_Position).xyz;
    vec3 p02 = (gl_in[8].gl_Position).xyz;
    vec3 p12 = (gl_in[9].gl_Position).xyz;
    vec3 p22 = (gl_in[10].gl_Position).xyz;
    vec3 p32 = (gl_in[11].gl_Position).xyz;
    vec3 p03 = (gl_in[12].gl_Position).xyz;
    vec3 p13 = (gl_in[13].gl_Position).xyz;
    vec3 p23 = (gl_in[14].gl_Position).xyz;
    vec3 p33 = (gl_in[15].gl_Position).xyz;

    // 曲面细分网格的朝向使得它位于 X-Z 平面中，
    // 因此 gl_TessCoord 的 X 和 Y 分量被应用于网格的 X 和 Z 坐标。
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // 三次贝塞尔基础函数
    float bu0 = (1.0-u)*(1.0-u)*(1.0-u);// (1-u)^3
    float bu1 = 3.0*u*(1.0-u)*(1.0-u);// 3u(1-u)^2
    float bu2 = 3.0*u*u*(1.0-u);// 3u^2(1-u)
    float bu3 = u*u*u;// u^3
    float bv0 = (1.0-v)*(1.0-v)*(1.0-v);// (1-v)^3
    float bv1 = 3.0*v*(1.0-v)*(1.0-v);// 3v(1-v)^2
    float bv2 = 3.0*v*v*(1.0-v);// 3v^2(1-v)
    float bv3 = v*v*v;// v^3

    // 输出曲面细分补丁中的顶点位置
    vec3 outputPosition =
        bu0 * (bv0*p00 + bv1*p01 + bv2*p02 + bv3*p03)
      + bu1 * (bv0*p10 + bv1*p11 + bv2*p12 + bv3*p13)
      + bu2 * (bv0*p20 + bv1*p21 + bv2*p22 + bv3*p23)
      + bu3 * (bv0*p30 + bv1*p31 + bv2*p32 + bv3*p33);
    gl_Position = mvp_matrix * vec4(outputPosition, 1.0f);

    // 输出插值过的纹理坐标
    // 注意：gl_TessCoord是曲面细分网格顶点的位置而非控制点的位置
    vec2 tc1 = mix(texCoord_TCSout[0], texCoord_TCSout[3], gl_TessCoord.x);
    vec2 tc2 = mix(texCoord_TCSout[12], texCoord_TCSout[15], gl_TessCoord.x);
    vec2 tc = mix(tc2, tc1, gl_TessCoord.y);
    texCoord_TESout = tc;
}

