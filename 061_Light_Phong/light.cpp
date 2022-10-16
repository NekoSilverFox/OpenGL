#include "light.h"

Light::Light()
{
    _length = LENGTH_LIGHT;
    color = QVector3D(1.0f, 1.0f, 1.0f);

    _genVectorVerticesAndIndices();
}


Light::Light(const float length, QVector3D color, QVector3D postion) :
    postion(postion),
    color(color),
    _length(length)
{
    _genVectorVerticesAndIndices();
}


void Light::_genVectorVerticesAndIndices()
{
    float r = _length / 2;
//    qDebug() << r;
    vertices = {
                -r, -r, -r,
                 r, -r, -r,
                 r,  r, -r,
                 r,  r, -r,
                -r,  r, -r,
                -r, -r, -r,

                -r, -r,  r,
                 r, -r,  r,
                 r,  r,  r,
                 r,  r,  r,
                -r,  r,  r,
                -r, -r,  r,

                -r,  r,  r,
                -r,  r, -r,
                -r, -r, -r,
                -r, -r, -r,
                -r, -r,  r,
                -r,  r,  r,

                 r,  r,  r,
                 r,  r, -r,
                 r, -r, -r,
                 r, -r, -r,
                 r, -r,  r,
                 r,  r,  r,

                -r, -r, -r,
                 r, -r, -r,
                 r, -r,  r,
                 r, -r,  r,
                -r, -r,  r,
                -r, -r, -r,

                -r,  r, -r,
                 r,  r, -r,
                 r,  r,  r,
                 r,  r,  r,
                -r,  r,  r,
                -r,  r, -r
                       };
}
