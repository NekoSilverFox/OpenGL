#include "cone.hpp"
#include "math.h"

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
    return indices.size() / 3;
}


void Cone::_genVectorVerticesAndIndices()
{
    float x_top = 0.0f;
    float y_top = _height;
    float z_top = 0.0f;

    vertices.push_back(x_top);
    vertices.push_back(y_top);
    vertices.push_back(z_top);

    /* 生成顶点数据 */
    float current_angle = 0.0f;
    for (int i = 0; i < 360/_step_angle; i++)
    {
        vertices.push_back(_r*cos(current_angle));  // x
        vertices.push_back(0.0f);                   // y
        vertices.push_back(_r*sin(current_angle));  // z

        current_angle += _step_angle;
    }

    /* 生成索引数据 */
    for (int i = 0; i < vertices.size(); i++)
    {
        indices.push_back(0);  // 顶点
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}
