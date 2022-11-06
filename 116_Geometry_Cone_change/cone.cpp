#include "cone.hpp"
#include "foxmath.h"
#include <math.h>
#include <QDebug>


Cone::Cone()
{
    _r = R;
    _height = HEIGHT;
    _step_angle = STEP_ANGLE;

    _genVectorVerticesAndIndices();
}


Cone::Cone(const float r, const float height, const float step_angle) :
    _r(r),
    _height(height),
    _step_angle(step_angle)
{
    _genVectorVerticesAndIndices();
}


unsigned int Cone::getNumTrianglesinSphere()
{
    return vertices.size() / 3;
}


void Cone::_genVectorVerticesAndIndices()
{
    /* 锥体顶点 (0-EBO)*/
    float top_x = 0.0f;
    float top_y = _height;
    float top_z = 0.0f;

    /* 生成锥体底部的顶点数据 */
    std::vector<float> tmp_vetices;
    float current_angle = 0.0f;
    for (int i = 0; i < 360/_step_angle + 1; i++)
    {
        tmp_vetices.push_back(_r*cos(current_angle*M_PI/180)); // x，【重点】sin() 默认是弧度！！，不是角度！！
        tmp_vetices.push_back(0.0f);                           // y
        tmp_vetices.push_back(_r*sin(current_angle*M_PI/180)); // z

        current_angle += _step_angle;
    }

    /* 推入侧边的顶点数据（位置 1：顶点坐标；位置 2：法线） */
    for (int i = 0; i < tmp_vetices.size() - 3; i += 3)
    {
//        float current_x = tmp_vetices[i + 0];
//        float current_y = tmp_vetices[i + 1];
//        float current_z = tmp_vetices[i + 2];

//        float next_x = tmp_vetices[i + 3];
//        float next_y = tmp_vetices[i + 4];
//        float next_z = tmp_vetices[i + 5];

//        QVector3D current_normal = -getNormalVector(QVector3D(top_x,     top_y,      top_z),
//                                                    QVector3D(current_x, current_y,  current_z),
//                                                    QVector3D(next_x,    next_y,     next_z));
//        qDebug() << "法线：" << current_normal;
        vertices.push_back(top_x);
        vertices.push_back(top_y);
        vertices.push_back(top_z);

        vertices.push_back(tmp_vetices[i + 0]);
        vertices.push_back(tmp_vetices[i + 1]);
        vertices.push_back(tmp_vetices[i + 2]);

        vertices.push_back(tmp_vetices[i + 3]);
        vertices.push_back(tmp_vetices[i + 4]);
        vertices.push_back(tmp_vetices[i + 5]);
    }




    /* 推入锥体地面的顶点数据（位置 1：顶点坐标；位置 2：法线） */
    float bottom_x = 0.0f;
    float bottom_y = 0.0f;
    float bottom_z = 0.0f;

    for (int i = 0; i < tmp_vetices.size() - 3; i += 3)
    {
//        float current_x = tmp_vetices[i + 0];
//        float current_y = tmp_vetices[i + 1];
//        float current_z = tmp_vetices[i + 2];

//        float next_x = tmp_vetices[i + 3];
//        float next_y = tmp_vetices[i + 4];
//        float next_z = tmp_vetices[i + 5];

//        QVector3D current_normal = getNormalVector(QVector3D(bottom_x,  bottom_y,   bottom_z),
//                                                   QVector3D(current_x, current_y,  current_z),
//                                                   QVector3D(next_x,    next_y,     next_z));
//        qDebug() << "法线：" << current_normal;
        vertices.push_back(bottom_x);
        vertices.push_back(bottom_y);
        vertices.push_back(bottom_z);

        vertices.push_back(tmp_vetices[i + 0]);
        vertices.push_back(tmp_vetices[i + 1]);
        vertices.push_back(tmp_vetices[i + 2]);


        vertices.push_back(tmp_vetices[i + 3]);
        vertices.push_back(tmp_vetices[i + 4]);
        vertices.push_back(tmp_vetices[i + 5]);
    }


}
