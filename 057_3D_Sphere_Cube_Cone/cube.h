#ifndef CUBE_H
#define CUBE_H


#include <vector>


const float LENGTH = 1.0f;


class Cube
{
public:
    Cube();
    Cube(const float length);

private:
    void _genVectorVerticesAndIndices();

public:
    std::vector<float> vertices;  // 顶点

private:
    float _length;
};

#endif // CUBE_H
