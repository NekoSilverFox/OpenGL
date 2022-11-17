#ifndef FUSIFORM_HPP
#define FUSIFORM_HPP

#include <vector>
#include <QMatrix4x4>


const float FUSIFORM_R       =   0.5;
const float FUSIFORM_HEIGHT  =   1.0;
const float FUSIFORM_STEP_ANGLE  =   90.0;

///
/// \brief 梭形
///

class Fusiform
{
public:
    Fusiform();
    Fusiform(const float r, const float height, const float step_angle);

    /* 获取绘制三角形的数量 */
    unsigned int getNumTrianglesinSphere();

private:
    void _genVectorVertices();

public:
    std::vector<QVector3D> vertexs;    // 顶点数据

    QMatrix4x4 _mat_model;

private:
    std::vector<QVector3D> _edge_vectors_top;  // 顶部边缘向量
    std::vector<QVector3D> _edge_vectors_middle;  // 中间边缘向量
    std::vector<QVector3D> _edge_vectors_bottom;  // 顶部边缘向量


    float _r;
    float _height;
    float _step_angle;  // 底的角度间隔，同时也是每旋转一次最大的角度
};

#endif // FUSIFORM_HPP
