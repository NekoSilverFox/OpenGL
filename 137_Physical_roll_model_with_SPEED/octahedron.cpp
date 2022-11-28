#include "octahedron.hpp"
#include <math.h>


//绕任意轴的旋转
QMatrix4x4 rotateArbitraryLine(QVector3D v1, QVector3D v2, float angle)
{
    QMatrix4x4 rmatrix;
    float theta = angle * M_PI / 180.0f;  // 转为弧度

    float a = v1.x();
    float b = v1.y();
    float c = v1.z();

    QVector3D p = (v2 - v1).normalized();

    float u = p.x();
    float v = p.y();
    float w = p.z();

    float uu = u * u;
    float uv = u * v;
    float uw = u * w;
    float vv = v * v;
    float vw = v * w;
    float ww = w * w;
    float au = a * u;
    float av = a * v;
    float aw = a * w;
    float bu = b * u;
    float bv = b * v;
    float bw = b * w;
    float cu = c * u;
    float cv = c * v;
    float cw = c * w;

    float costheta = cos(theta);
    float sintheta = sin(theta);


    rmatrix(0, 0) = uu + (vv + ww) * costheta;
    rmatrix(0, 1) = uv * (1 - costheta) + w * sintheta;
    rmatrix(0, 2) = uw * (1 - costheta) - v * sintheta;
    rmatrix(0, 3) = 0;

    rmatrix(1, 0) = uv * (1 - costheta) - w * sintheta;
    rmatrix(1, 1) = vv + (uu + ww) * costheta;
    rmatrix(1, 2) = vw * (1 - costheta) + u * sintheta;
    rmatrix(1, 3) = 0;

    rmatrix(2, 0) = uw * (1 - costheta) + v * sintheta;
    rmatrix(2, 1) = vw * (1 - costheta) - u * sintheta;
    rmatrix(2, 2) = ww + (uu + vv) * costheta;
    rmatrix(2, 3) = 0;

    rmatrix(3, 0) = (a * (vv + ww) - u * (bv + cw)) * (1 - costheta) + (bw - cv) * sintheta;
    rmatrix(3, 1) = (b * (uu + ww) - v * (au + cw)) * (1 - costheta) + (cu - aw) * sintheta;
    rmatrix(3, 2) = (c * (uu + vv) - w * (au + bv)) * (1 - costheta) + (av - bu) * sintheta;
    rmatrix(3, 3) = 1;

    return rmatrix;
}

/// 弧度转角度
/// \brief sina
/// \param range
/// \return
///
float to_angle(float range)
{
    return range * 180.0f/M_PI;
}

QVector3D rotateVector3D(QVector3D vertex3D, QMatrix4x4 mat_rotate)
{
    QVector4D vertex4D = vertex3D.toVector4D();
    vertex4D.setW(1.0f);
    vertex4D = mat_rotate * vertex4D;

    return vertex4D.toVector3D();
}


Octahedron::Octahedron()
{
    this->_r            = OCTAHEDRON_R;
    this->_height       = OCTAHEDRON_HEIGHT;
    this->_step_angle   = OCTAHEDRON_STEP_ANGLE;

    _max_put_down_angle = atan2f(_height, _r) * 180.0f/M_PI + 10;
    _current_put_down_angle = 0.0f;
    _genVectorVertices();
}



Octahedron::Octahedron(const float r, const float height, const float step_angle) :
    _r(r),
    _height(height),
    _step_angle(step_angle)
{
    _max_put_down_angle = atan2f(_height, _r) * 180.0f/M_PI + 10;
    _current_put_down_angle = 0.0f;

    _genVectorVertices();
}



void Octahedron::_genVectorVertices()
{
    QMatrix4x4 mat_rotate; mat_rotate.rotate(_max_put_down_angle + 10, 1.0f, 0.0f, 1.0f);


    QVector3D vertex_top    = QVector3D(0.0f, 2 * _height,  0.0f);   // 最顶点
    QVector3D vertex_bottom = QVector3D(0.0f, 0.0,          0.0f);   // 最底点
    QVector3D vertex_center = QVector3D(0.0f, 1 * _height,  0.0f);   // 中心

    // 生成中央环上的顶点
    std::vector<QVector3D> tmp_vetices;
    for (float angle = 0.0f; angle <= 360; angle += _step_angle)
    {
        QVector3D vertex3D = QVector3D(
                    _r * cos(angle*M_PI/180.0f),   // x
                    vertex_center.y(),             // y
                    _r * sin(angle*M_PI/180.0f));  // z

        tmp_vetices.push_back(vertex3D);
    }

    // 存入顶点
    for (auto i = 0; i < tmp_vetices.size() - 1; i++)
    {
        vertexs.push_back(vertex_top);
        vertexs.push_back(tmp_vetices[i + 0]);
        vertexs.push_back(tmp_vetices[i + 1]);

        vertexs.push_back(tmp_vetices[i + 0]);
        vertexs.push_back(vertex_bottom);
        vertexs.push_back(tmp_vetices[i + 1]);
    }

    // 梭形的边缘（向量）
    for (auto i = 0; i < tmp_vetices.size(); i++)
    {
        _edge_vectors_top.push_back(tmp_vetices[i]          - vertex_top);
        _edge_vectors_middle.push_back(tmp_vetices[i + 1]   - tmp_vetices[i + 0]);
        _edge_vectors_bottom.push_back(tmp_vetices[i]       - vertex_bottom);
    }

}



/** 放倒梭形
 * @brief Octahedron::putDown
 */
bool Octahedron::putDown(float angle)
{
    if (_current_put_down_angle >= _max_put_down_angle) return false;

    speed_role++;
    float speed_plus = speed_role / 10; // 每次旋转由慢到快
    if (_current_put_down_angle + speed_plus + angle >= _max_put_down_angle)
    {
        speed_plus = 0.0f;
        speed_role = 0;
    }

    _mat_model.rotate(angle + speed_plus, _edge_vectors_bottom[2] - _edge_vectors_bottom[1]);
    _current_put_down_angle += (angle + speed_plus);

    return true;
}

bool Octahedron::roleByEdge(const float angle)
{
    if (_current_rotate_drop_angle >= 90)
    {
        ready2drop = true;
        return false;
    }

    speed_role--;
    float speed_plus = speed_role / 20; // 每次旋转由慢到快
    if (_current_rotate_angle + speed_plus + angle >= 70.0f) speed_plus = 0.0f;

    if (role_time == 3)
    {
        if (_current_rotate_drop_angle + speed_plus + 1 >= 90.0f) speed_plus = 0.0f;
        _mat_model = rotateArbitraryLine(_edge_vectors_bottom[1], _edge_vectors_bottom[2], 1 + abs(speed_plus)) * _mat_model;
        _current_rotate_drop_angle += 1 + abs(speed_plus);

        return false;
    }

    int i_role_edge = 4 - current_edge % 4;
    if (_current_rotate_angle >= 70.0f)
    {
        _current_rotate_angle = 0.0f;
        speed_role = 0;
        current_edge++;
        role_time++;
    }


    _current_rotate_angle += abs(angle + speed_plus);
    _mat_model.rotate(angle + speed_plus, _edge_vectors_bottom[i_role_edge]);
    qDebug() << "转轴 index：" << i_role_edge << "当前角度：" << _current_rotate_angle;

    return true;
}
