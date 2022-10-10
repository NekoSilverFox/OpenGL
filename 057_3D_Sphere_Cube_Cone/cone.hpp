#ifndef CONE_HPP
#define CONE_HPP


#include <vector>
#include <QMatrix4x4>

const float R       =   0.5;
const float HEIGHT  =   1.0;
const float STEP_ANGLE  =   1.0;


class Cone
{
public:
    Cone();
    Cone(const float r, const float height, const float step_angle);

    /* 获取绘制三角形的数量 */
    unsigned int getNumTrianglesinSphere();


private:
    void _genVectorVerticesAndIndices();


public:
    std::vector<float> vertices;    // 顶点
    std::vector<int> indices;       // EBO 索引s

    QMatrix4x4 mat_model;

private:
    float _r;       // 半径
    float _height;  // 高度
    float _step_angle;  // 底的角度间隔

};

#endif // CONE_HPP
