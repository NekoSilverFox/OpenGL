#include "light.h"

Light::Light()
{
    _length = LENGTH_LIGHT;
    postion = QVector3D(1.2f, 0.5f, 2.0f);
    color_ambient   = QVector3D(0.2f, 0.2f, 0.2f);
    color_diffuse   = QVector3D(0.5f, 0.5f, 0.5f);
    color_specular  = QVector3D(1.0f, 1.0f, 1.0f);
    color_shininess = 128.0f;

    _genVectorVerticesAndIndices();
}


Light::Light(const float length,
             QVector3D postion,
             QVector3D color_ambient   ,
             QVector3D color_diffuse   ,
             QVector3D color_specular  ,
             float     color_shininess     ) :
    postion(postion),
    color_ambient (color_ambient),
    color_diffuse (color_diffuse),
    color_specular(color_specular),
    color_shininess(color_shininess),
    _length(length)
{
    _genVectorVerticesAndIndices();
}


void Light::_genVectorVerticesAndIndices()
{
    float r = _length / 2;

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
