#ifndef BEZIERFACE_HPP
#define BEZIERFACE_HPP


#include <vector>
#include <QVector3D>
#include <QVector2D>

class BezierFace
{

public:
    BezierFace();
    BezierFace(int step, float controlPoints[], int prec);
    int getNumVertices();
    int getNumIndices();
    std::vector<QVector3D> getVertices();
    std::vector<QVector3D> getNormals();
    std::vector<QVector2D> getTexCoords();
    std::vector<int> getIndices();

public:
    int numVertices;  //顶点个数

    int numIndices;  //索引个数

    std::vector<QVector3D> vertices;  //顶点

    std::vector<QVector3D> normals;  //法线

    std::vector<QVector2D> texCoords;  //纹理

    std::vector<int> indices;  //索引

    void init(int prec);  //计算数据

    float* controlPoints;  //控制点
    std::vector<QVector3D> controlPointsVector;


    int step;//曲线阶数

    float toRadians(float degrees);
    float Bernstein(float u, int index);


};

#endif // BEZIERFACE_HPP
