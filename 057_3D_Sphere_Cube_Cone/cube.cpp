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

