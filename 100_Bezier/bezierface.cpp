#include "bezierface.hpp"


BezierFace::BezierFace(){}


BezierFace::BezierFace(int step, float controlPoints[], int prec)
{
    this->step = step;
    this->controlPoints = controlPoints;
    init(prec);
}


/** 通过要划分的网格的区块，初始化以下变量：
 *  - vertices
 *  - normals
 *  - texCoords
 *  - indices
 *  - controlPointsVector
 *
 * @brief BezierFace::init
 * @param prec
 */
void BezierFace::init(int prec)  // prec 应该代表分割成 prec*prec 的方形网格
{
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;

    /* 初始化网格，但里面都是 .0 */
    for (int i = 0; i < numVertices; i++)
    {
        vertices.push_back(QVector3D());
        normals.push_back(QVector3D());
        texCoords.push_back(QVector2D());
    }

    /* 初始化索引数组，但里面都是 .0 */
    for (int i = 0; i < numIndices; i++)
    {
        indices.push_back(0);
    }


    /* 把控制点转换为向量，并传到类的 controlPointsVector 数组里 */
    for (int i = 0; i < (step + 1) * (step + 1) * 3; i += 3)
    {
        controlPointsVector.push_back(
                    QVector3D(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]));
    }

    for (int i = 0; i <= prec; i++)
    {
        for (int j = 0; j <= prec; j++)
        {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            float u = (float)i / prec;
            float v = (float)j / prec;

            for (int i = 0; i <= 3; i++)
            {
                for (int j = 0; j <= 3; j++)
                {
                    int index = i * 4 + j;
                    x += controlPointsVector[index].x() * Bernstein(u, i) * Bernstein(v, j);
                    y += controlPointsVector[index].y() * Bernstein(u, i) * Bernstein(v, j);
                    z += controlPointsVector[index].z() * Bernstein(u, i) * Bernstein(v, j);
                }
            }
            vertices [i * (prec + 1) + j]   =   QVector3D(x, y, z);
            normals  [i * (prec + 1) + j]   =   QVector3D(x, y, z);
            texCoords[i * (prec + 1) + j]   =   QVector2D((float)j / prec,	 (float)i / prec);
        }
    }

    //计算索引
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i       * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i       * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i       * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}


/** 将角度转换为弧度
 * @brief BezierFace::toRadians
 * @param degrees
 * @return
 */
float BezierFace::toRadians(float degrees) { return (degrees * M_PI) / 180.0f; }


float BezierFace::Bernstein(float t, int index)
{
    switch (index)
    {
    default:
    case 0:
        return pow(1.0 - t, 3);
        break;

    case 1:
        return 3 * t * pow(1.0 - t, 2);
        break;

    case 2:
        return 3 * pow(t, 2) * (1 - t);
        break;

    case 3:
        return pow(t, 3);
        break;
    }

}


int BezierFace::getNumVertices()
{
    return numVertices;
}


int BezierFace::getNumIndices()
{
    return numIndices;
}


std::vector<QVector3D> BezierFace::getVertices()
{
    return vertices;
}


std::vector<QVector3D> BezierFace::getNormals()
{
    return normals;
}


std::vector<QVector2D> BezierFace::getTexCoords()
{
    return texCoords;
}


std::vector<int> BezierFace::getIndices()
{
    return indices;
}

