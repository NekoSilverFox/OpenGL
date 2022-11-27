#ifndef OCTAHEDRON_H
#define OCTAHEDRON_H

#include <vector>
#include <QMatrix4x4>

const float OCTAHEDRON_R       =   0.5;
const float OCTAHEDRON_HEIGHT  =   1.0;
const float OCTAHEDRON_STEP_ANGLE  =   90.0;


enum RoleEdge
{
    Top,
    Middle,
    Bottom
};

///
/// \brief 梭形
///
class Octahedron
{
public:
    Octahedron();
    Octahedron(const float r, const float height, const float step_angle);

    /* 获取绘制三角形的数量 */
    unsigned int getNumTrianglesinSphere();

    bool putDown(float angle);  // 将梭形放倒
    bool roleByEdge(const float angle);
    unsigned int maxRoleIndex();
    float maxRoleAngle();

private:
    void _genVectorVertices();

public:
    std::vector<QVector3D> vertexs;    // 顶点数据

    QMatrix4x4 _mat_model;

//private:
    std::vector<QVector3D> _edge_vectors_top;  // 顶部边缘向量
    std::vector<QVector3D> _edge_vectors_middle;  // 中间边缘向量
    std::vector<QVector3D> _edge_vectors_bottom;  // 顶部边缘向量

    float _r;
    float _height;
    float _step_angle;  // 底的角度间隔，同时也是每旋转一次最大的角度

    float _max_put_down_angle;  // 放倒需要沿着 x 轴转最多转少角度
    float _current_put_down_angle;

    float _current_rotate_angle = 0.0f;
    int current_edge = 1;
    int role_time = 0;

    int speed_role = 0;  // 每次旋转由慢到快(给旋转一加速度)

    float _current_rotate_drop_angle = 0.0f;

    bool ready2drop = false;
};

#endif // OCTAHEDRON_H
