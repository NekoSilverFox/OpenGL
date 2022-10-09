#ifndef SPHERE_H
#define SPHERE_H


#include <vector>
#include <QMatrix4x4>


//将球横纵划分成50*50的网格
const unsigned int X_SPHERE_SEGMENTS = 50;
const unsigned int Y_SPHERE_SEGMENTS = 50;


class Sphere
{
public:
    Sphere();
    Sphere(const unsigned int x_segments, const unsigned int y_segments);

    /* 获取绘制三角形的数量 */
    unsigned int getNumTrianglesinSphere();

private:
    void _genVectorVerticesAndIndices();


public:
    std::vector<float> vertices;  // 顶点
    std::vector<int> indices; // EBO 索引s

    QMatrix4x4 mat_model;

private:
    unsigned int _x_segments;
    unsigned int _y_segments;
};

#endif // SPHERE_H
