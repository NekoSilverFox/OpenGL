#ifndef CUBE_H
#define CUBE_H


#include <vector>
#include <QMatrix4x4>

const float LENGTH = 1.0f;
const QVector3D COLOR_CUBE = QVector3D(1.0f, 0.5f, 0.3f);

class Cube
{
public:
    Cube();
    Cube(const float length, QVector3D color = COLOR_CUBE);

private:
    void _genVectorVerticesAndIndices();

public:
    std::vector<float> vertices;  // 顶点

    QMatrix4x4 mat_model;
    QVector3D color;

private:
    float _length;
};

#endif // CUBE_H
