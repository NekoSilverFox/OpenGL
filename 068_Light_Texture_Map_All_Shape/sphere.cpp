#include "sphere.hpp"
#include "foxmath.h"
#include <math.h>


Sphere::Sphere() :
    _r(R_SPHERE),
    _step_angle(STEP_ANGLE_SPHERE)
{
    _genVectorVerticesAndIndices();
}


Sphere::Sphere(const float r, const float step_angle) :
    _r(r),
    _step_angle(step_angle)
{
    _genVectorVerticesAndIndices();
}


/* 获取绘制三角形的数量 */
unsigned int Sphere::getNumTrianglesinSphere()
{
    return vertices.size() / 6;
}



void Sphere::_genVectorVerticesAndIndices()
{
    /* 储存球上的所有的点 */
    std::vector<float> points;


    /* 绘制上半球的所有顶点 */
    const unsigned int num_point_circle = 360.0f / _step_angle; // 每个圈要绘制几个点
    const unsigned int num_circle = 360.0 / _step_angle;  // 半球上需要绘制圈圈的个数

    /* 开始从最底部处一圈一圈的向上绘制，注意：最底部的点都是重叠了的 */
    float current_xy_angle = -180.0f;
    for (int i = 0; i < num_circle; i++)
    {
        float current_xz_angle = 0.0f;  // 水平角度

        /* 圈上的点 */
        for (int j = 0; j <= num_point_circle; j++)
        {
            points.push_back(_r * sin(current_xz_angle*M_PI/180) * cos(current_xy_angle*M_PI/180));     // x，【重点】sin() 默认是弧度！！，不是角度！！
            points.push_back(_r * sin(current_xy_angle*M_PI/180));                                      // y
            points.push_back(_r * cos(current_xz_angle*M_PI/180) * cos(current_xy_angle*M_PI/180));     // z

            current_xz_angle += _step_angle;
            qDebug() << "生成本圈上的第：" << j << "个点";
        }

        current_xy_angle += _step_angle;
    }

//    this->vertices = points;
    int num_points = 3 * (num_point_circle * num_circle - num_point_circle);

    /// 垃圾值
    float current_x;
    float current_y;
    float current_z;
    QVector3D current_normal;

    for (int i = 0; i <= num_points; i += 3)
    {
        if (i % num_point_circle == 3) continue;
#if 0
        /* 正三角 */
        float current_x = points[i + 0];
        float current_y = points[i + 1];
        float current_z = points[i + 2];


        float next_x = points[i + 3];
        float next_y = points[i + 4];
        float next_z = points[i + 5];


        float next_top_x = points[i + 3 * num_point_circle + 3];
        float next_top_y = points[i + 3 * num_point_circle + 4];
        float next_top_z = points[i + 3 * num_point_circle + 5];

        QVector3D current_normal =  getNormalVector(QVector3D(current_x,    current_y,      current_z),
                                                    QVector3D(next_x,       next_y,         next_z),
                                                    QVector3D(next_top_x,   next_top_y,     next_top_z));  // 法线

        vertices.push_back(current_x);
        vertices.push_back(current_y);
        vertices.push_back(current_z);
        vertices.push_back(current_normal.x());
        vertices.push_back(current_normal.y());
        vertices.push_back(current_normal.z());

        vertices.push_back(next_x);
        vertices.push_back(next_y);
        vertices.push_back(next_z);
        vertices.push_back(current_normal.x());
        vertices.push_back(current_normal.y());
        vertices.push_back(current_normal.z());

        vertices.push_back(next_top_x);
        vertices.push_back(next_top_y);
        vertices.push_back(next_top_z);
        vertices.push_back(current_normal.x());
        vertices.push_back(current_normal.y());
        vertices.push_back(current_normal.z());
#endif
#if 1
        /* 倒三角 */
        current_x = points[i + 0];
        current_y = points[i + 1];
        current_z = points[i + 2];

        float current_top_x = points[i + 3 * num_point_circle + 0];
        float current_top_y = points[i + 3 * num_point_circle + 1];
        float current_top_z = points[i + 3 * num_point_circle + 2];


        float current_top_right_x = points[i + 3 * num_point_circle + 3];
        float current_top_right_y = points[i + 3 * num_point_circle + 4];
        float current_top_right_z = points[i + 3 * num_point_circle + 5];

        current_normal =  getNormalVector(QVector3D(current_x,              current_y,              current_z),
                                          QVector3D(current_top_x,          current_top_y,          current_top_z),
                                          QVector3D(current_top_right_x,    current_top_right_y,    current_top_right_z));


        vertices.push_back(current_x);
        vertices.push_back(current_y);
        vertices.push_back(current_z);
        vertices.push_back(current_normal.x());
        vertices.push_back(current_normal.y());
        vertices.push_back(current_normal.z());

        vertices.push_back(current_top_x);
        vertices.push_back(current_top_y);
        vertices.push_back(current_top_z);
        vertices.push_back(current_normal.x());
        vertices.push_back(current_normal.y());
        vertices.push_back(current_normal.z());

        vertices.push_back(current_top_right_x);
        vertices.push_back(current_top_right_y);
        vertices.push_back(current_top_right_z);
        vertices.push_back(current_normal.x());
        vertices.push_back(current_normal.y());
        vertices.push_back(current_normal.z());
#endif
    }


}
