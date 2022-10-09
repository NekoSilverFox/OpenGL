#include "sphere.hpp"
#include <math.h>


Sphere::Sphere() :
    _x_segments(X_SPHERE_SEGMENTS),
    _y_segments(Y_SPHERE_SEGMENTS)
{
    _genVectorVerticesAndIndices();
}


Sphere::Sphere(const unsigned int x_segments, const unsigned int y_segments) :
    _x_segments(x_segments),
    _y_segments(y_segments)
{
    _genVectorVerticesAndIndices();
}


/* 获取绘制三角形的数量 */
unsigned int Sphere::getNumTrianglesinSphere()
{
    return _x_segments * _y_segments * 6;
}


void Sphere::_genVectorVerticesAndIndices()
{
    /*2-计算球体顶点*/
    //生成球的顶点
    for (int y = 0; y <= _y_segments; y++)
    {
        for (int x = 0; x <= _x_segments; x++)
        {
            float xSegment = (float)x / (float)_x_segments;
            float ySegment = (float)y / (float)_y_segments;
            float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            float yPos = cos(ySegment * M_PI);
            float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
        }
    }

    //生成球的Indices
    for (int i = 0; i < _y_segments; i++)
    {
        for (int j = 0; j < _x_segments; j++)
        {
            indices.push_back(i * (_x_segments + 1) + j);
            indices.push_back((i + 1) * (_x_segments + 1) + j);
            indices.push_back((i + 1) * (_x_segments + 1) + j+1);
            indices.push_back(i* (_x_segments + 1) + j);
            indices.push_back((i + 1) * (_x_segments + 1) + j + 1);
            indices.push_back(i * (_x_segments + 1) + j + 1);
        }
    }
}
