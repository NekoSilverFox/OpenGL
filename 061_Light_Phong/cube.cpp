#include "cube.hpp"

Cube::Cube()
{
    _length = LENGTH;

    _genVectorVerticesAndIndices();
}


Cube::Cube(const float length) :
    _length(length)
{
    _genVectorVerticesAndIndices();
}


void Cube::_genVectorVerticesAndIndices()
{
    float r = _length / 2;

    vertices = {
        -r, -r, -r,  0.0f,  0.0f, -1.0f,
         r, -r, -r,  0.0f,  0.0f, -1.0f,
         r,  r, -r,  0.0f,  0.0f, -1.0f,
         r,  r, -r,  0.0f,  0.0f, -1.0f,
        -r,  r, -r,  0.0f,  0.0f, -1.0f,
        -r, -r, -r,  0.0f,  0.0f, -1.0f,

        -r, -r,  r,  0.0f,  0.0f, 1.0f,
         r, -r,  r,  0.0f,  0.0f, 1.0f,
         r,  r,  r,  0.0f,  0.0f, 1.0f,
         r,  r,  r,  0.0f,  0.0f, 1.0f,
        -r,  r,  r,  0.0f,  0.0f, 1.0f,
        -r, -r,  r,  0.0f,  0.0f, 1.0f,

        -r,  r,  r, -1.0f,  0.0f,  0.0f,
        -r,  r, -r, -1.0f,  0.0f,  0.0f,
        -r, -r, -r, -1.0f,  0.0f,  0.0f,
        -r, -r, -r, -1.0f,  0.0f,  0.0f,
        -r, -r,  r, -1.0f,  0.0f,  0.0f,
        -r,  r,  r, -1.0f,  0.0f,  0.0f,

         r,  r,  r,  1.0f,  0.0f,  0.0f,
         r,  r, -r,  1.0f,  0.0f,  0.0f,
         r, -r, -r,  1.0f,  0.0f,  0.0f,
         r, -r, -r,  1.0f,  0.0f,  0.0f,
         r, -r,  r,  1.0f,  0.0f,  0.0f,
         r,  r,  r,  1.0f,  0.0f,  0.0f,

        -r, -r, -r,  0.0f, -1.0f,  0.0f,
         r, -r, -r,  0.0f, -1.0f,  0.0f,
         r, -r,  r,  0.0f, -1.0f,  0.0f,
         r, -r,  r,  0.0f, -1.0f,  0.0f,
        -r, -r,  r,  0.0f, -1.0f,  0.0f,
        -r, -r, -r,  0.0f, -1.0f,  0.0f,

        -r,  r, -r,  0.0f,  1.0f,  0.0f,
         r,  r, -r,  0.0f,  1.0f,  0.0f,
         r,  r,  r,  0.0f,  1.0f,  0.0f,
         r,  r,  r,  0.0f,  1.0f,  0.0f,
        -r,  r,  r,  0.0f,  1.0f,  0.0f,
        -r,  r, -r,  0.0f,  1.0f,  0.0f
                       };
}

