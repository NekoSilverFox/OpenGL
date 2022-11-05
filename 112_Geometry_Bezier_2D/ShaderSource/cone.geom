#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 g_axis_normal;
    vec3 g_axis_fragPos;
    int id;
} gs_in[];  // 注意，这里是对于图元来说的。也就是三角形的各个点


out vec3 res_g_axis_normal;
out vec3 res_g_axis_fragPos;


uniform float time;

const int Segments = 20
const int SegmentsMax = 30; // max_vertices = (SegmentsMax+1)*4;
const int SegmentsMin = 3; // min number of segments per curve


vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 200.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}


// https://vicrucann.github.io/tutorials/bezier-shader/
vec4 toBezier(float delta, int i, vec4 P0, vec4 P1, vec4 P2, vec4 P3)
{
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;

    return (  P0 * one_minus_t2 * one_minus_t
            + P1 * 3.0          * t             * one_minus_t2
            + P2 * 3.0          * t2            * one_minus_t
            + P3 * t2           * t);
}

void main() {


//    const int max_len = 20;                   //使用20条线来绘制一段bezier曲线

//    vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.5);     //取前两个点的中点
//    vec4 p1;
//    vec4 p2;
//    if(gs_in[2].id % 2 == 0){
//        p1 = gl_in[1].gl_Position;                                    //调整点
//        p2 = mix(gl_in[1].gl_Position, gl_in[2].gl_Position, 0.5);      //取后两个点的中点
//    }
//    else{
//        p1 = gl_in[0].gl_Position;                                    //同上
//        p2 = mix(gl_in[0].gl_Position, gl_in[2].gl_Position, 0.5);
//    }

//    for(int i = 0; i<= max_len; i++){                                    //根据公式生成顶点，并提交
//        float t = i*0.05;
//        gl_Position=p0*(1.0-t)*(1.0-t)+p1*2*t*(1.0-t)+p2*t*t;
//        res_g_axis_normal   = gs_in[0].g_axis_normal;
//        res_g_axis_fragPos  = gs_in[0].g_axis_fragPos;
//        EmitVertex();
//    }

//    EndPrimitive();


/* cut segments number if larger or smaller than allowed */
    int nSegments = (Segments > SegmentsMax)? SegmentsMax : Segments;
    nSegments = (nSegments < SegmentsMin)? SegmentsMin: nSegments;


// 4 control points
    vec4 B[4];
    B[0] = gl_in[0].gl_Position;
    B[1] = gl_in[1].gl_Position;
    B[2] = gl_in[2].gl_Position;
    B[3] = gl_in[3].gl_Position;


/* use the points to build a bezier line */
   float delta = 1.0 / float(nSegments);
   vec4 Points[4]; // segments of curve in 3d
   float zValues[4];
   int j = 0; // bezier segment index for color interpolation
   for (int i=0; i<=nSegments; ++i){
       /* first point */
       if (i==0){
           Points[1] = toBezier(delta, i,   B[0], B[1], B[2], B[3]);
           Points[2] = toBezier(delta, i+1, B[0], B[1], B[2], B[3]);
           Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
           vec4 dir = normalize(Points[1] - Points[2]);
           Points[0] = Points[1] + dir * 0.01;
       }
       else if (i < nSegments-1){
           Points[0] = Points[1];
           Points[1] = Points[2];
           Points[2] = Points[3];
           Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
       }
       /* last point */
       else {
           Points[0] = Points[1];
           Points[1] = Points[2];
           Points[2] = Points[3];
           vec4 dir = normalize(Points[2] - Points[1]);
           Points[3] = Points[2] + dir * 0.01;
       }



   }

    gl_Position

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
