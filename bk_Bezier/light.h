#ifndef LIGHT_H
#define LIGHT_H
#include <QVector3D>
#include <QMatrix4x4>


const float LENGTH_LIGHT = 1.0f;


class Light
{
public:
    Light();
    Light(const float length, QVector3D postion,
          QVector3D color_ambient   = QVector3D(0.2f, 0.2f, 0.2f),
          QVector3D color_diffuse   = QVector3D(0.5f, 0.5f, 0.5f),
          QVector3D color_specular  = QVector3D(1.0f, 1.0f, 1.0f),
          float color_shininess     = 128.0f);


private:
    void _genVectorVerticesAndIndices();


public:
    QMatrix4x4 mat_model;
    QVector3D postion;

    QVector3D color_ambient;
    QVector3D color_diffuse;
    QVector3D color_specular;
    float color_shininess;

    std::vector<float> vertices;  // 顶点


private:
    float _length;
};

#endif // LIGHT_H
