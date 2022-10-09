#include "cone.hpp"
#include "math.h"
#include <QDebug>

Cone::Cone()
{
    _r = R;
    _height = HEIGHT;
    _step_angle = STEP_ANGLE;

    _genVectorVerticesAndIndices();
}


Cone::Cone(const float r, const float height, const float step_angle) :
    _r(r),
    _height(height),
    _step_angle(step_angle)
{
    _genVectorVerticesAndIndices();
}


unsigned int Cone::getNumTrianglesinSphere()
{
    return indices.size();
}


void Cone::_genVectorVerticesAndIndices()
{

    /* 锥体顶点 (0-EBO)*/
    vertices.push_back( 0.0f);
    vertices.push_back(_height);
    vertices.push_back(0.0f);


    /* 锥体底部中心点，用于绘制底部 (1-EBO)*/
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);


    /* 生成锥体底部的顶点数据 (EBO 索引从2开始)*/
    float current_angle = 0.0f;
    for (int i = 0; i < 360/_step_angle + 1; i++)
    {
        vertices.push_back(_r*cos(current_angle*M_PI/180)); // x，【重点】sin() 默认是弧度！！，不是角度！！
        vertices.push_back(0.0f);                           // y
        vertices.push_back(_r*sin(current_angle*M_PI/180)); // z

        current_angle += _step_angle;
    }

    /* 生成索引数据 */
    for (int i = 0; i < vertices.size() - 2; i++)
    {
        indices.push_back(0);  // 锥体顶点
        indices.push_back(i + 2);
        indices.push_back(i + 3);

        indices.push_back(1);  // 锥体底部中心点
        indices.push_back(i + 2);
        indices.push_back(i + 3);
    }
}
