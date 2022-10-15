#ifndef LIGHT_H
#define LIGHT_H
#include <QVector3D>
#include <QMatrix4x4>


const float LENGTH_LIGHT = 1.0f;


class Light
{
public:
    Light();
    Light(const float length, QVector3D color);


private:
    void _genVectorVerticesAndIndices();


public:
    QMatrix4x4 mat_model;
    QVector3D color;
    std::vector<float> vertices;  // 顶点


private:
    float _length;
};

#endif // LIGHT_H
