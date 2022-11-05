#version 330 core
layout (triangles) in;                      //以三角形为单位进行扩张，每次从VertexShader往GeomShader传进三个点进行数据处理
layout (line_strip, max_vertices = 32) out; //将一个点变为最多32个可连成线条的点 交给FragShader



in VS_OUT {
    int id;
} gs_in[];


void creatBezier(){

    const int max_len = 20;                   //使用20条线来绘制一段bezier曲线

    vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.5);     //取前两个点的中点
    vec4 p1;
    vec4 p2;
    if(gs_in[2].id % 2 == 0){
        p1 = gl_in[1].gl_Position;                                    //调整点
        p2 = mix(gl_in[1].gl_Position, gl_in[2].gl_Position, 0.5);      //取后两个点的中点
    }
    else{
        p1 = gl_in[0].gl_Position;                                    //同上
        p2 = mix(gl_in[0].gl_Position, gl_in[2].gl_Position, 0.5);
    }

    for(int i = 0; i<= max_len; i++){                                    //根据公式生成顶点，并提交
        float t = i*0.05;
        gl_Position=p0*(1.0-t)*(1.0-t)+p1*2*t*(1.0-t)+p2*t*t;
        EmitVertex();
    }

    EndPrimitive();
}


void main(){
    creatBezier();
}
