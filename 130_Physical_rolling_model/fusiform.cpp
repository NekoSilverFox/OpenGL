#include "fusiform.hpp"
#include <math.h>

Fusiform::Fusiform()
{
    this->_r = FUSIFORM_R;
    this->_height = FUSIFORM_HEIGHT;
    this->_step_angle = FUSIFORM_STEP_ANGLE;

    _max_put_down_angle = atan2f(_height, _r) * 180.0f/M_PI;
    _current_put_down_angle = 0.0f;

    _max_role_angle = FUSIFORM_STEP_ANGLE;
    _current_role_angle = 0.0f;
    current_edge = RoleEdge::Bottom;
    current_index_edge = 1;

    _genVectorVertices();
}

Fusiform::Fusiform(const float r, const float height, const float step_angle) :
    _r(r),
    _height(height),
    _step_angle(step_angle)
{
    _max_put_down_angle = atan2f(_height, _r) * 180.0f/M_PI;
    _current_put_down_angle = 0.0f;

    _max_role_angle = step_angle;
    _current_role_angle = 0.0f;

    current_edge = RoleEdge::Bottom;
    current_index_edge = 1;

    _genVectorVertices();
}

void Fusiform::_genVectorVertices()
{
    // 最顶点
    QVector3D vertex_top = QVector3D(0.0f, _height, 0.0f);

    // 最底点
    QVector3D vertex_bottom = QVector3D(0.0f, -_height, 0.0f);

    // 中心
    QVector3D vertex_center = QVector3D(0.0f, 0.0f, 0.0f);

    // 生成中央环上的顶点
    std::vector<QVector3D> tmp_vetices;
    for (float angle = 0.0f; angle <= 360; angle += _step_angle)
    {
        tmp_vetices.push_back(QVector3D(
                                  _r * cos(angle*M_PI/180.0f),   // x
                                  vertex_center.y(),             // y
                                  _r * sin(angle*M_PI/180.0f))); // z
    }

    // 存入顶点，及向量
    for (int i = 0; i < tmp_vetices.size() - 1; i++)
    {
        vertexs.push_back(vertex_top);
        vertexs.push_back(tmp_vetices[i + 0]);
        vertexs.push_back(tmp_vetices[i + 1]);

        vertexs.push_back(tmp_vetices[i + 0]);
        vertexs.push_back(vertex_bottom);
        vertexs.push_back(tmp_vetices[i + 1]);
    }

    // 梭形的边缘（向量）
    for (int i = 0; i < tmp_vetices.size(); i++)
    {
        _edge_vectors_top.push_back(vertex_top - tmp_vetices[i]);
        _edge_vectors_middle.push_back(tmp_vetices[i + 1] - tmp_vetices[i + 0]);
        _edge_vectors_bottom.push_back(vertex_bottom - tmp_vetices[i]);
    }

}

/** 放倒梭形
 * @brief Fusiform::putDown
 */
bool Fusiform::putDown(float angle)
{
    if (_current_put_down_angle >= _max_put_down_angle) return false;

    _mat_model.rotate(angle, 1.0f, 0.0f, -1.0f);
    _current_put_down_angle += angle;

    return true;
}

unsigned int Fusiform::maxRoleIndex() {return _edge_vectors_bottom.size();}


bool Fusiform::roleByEdge(RoleEdge edge, const unsigned int index_edge, const float angle)
{
    if (angle + _current_role_angle > _max_role_angle || index_edge >= _edge_vectors_bottom.size())
    {
        qDebug() << "[ERROR] Can not role, angle will bigger than _max_role_angle" << _max_role_angle << "or index bigger than " << _edge_vectors_bottom.size();
        return false;
    }

    if (edge != _current_role_angle) _current_role_angle = 0.0f;

    switch (edge)
    {
    case RoleEdge::Top:
        _mat_model.rotate(angle, _edge_vectors_top[index_edge]);
        break;

    case RoleEdge::Middle:
        _mat_model.rotate(angle, _edge_vectors_middle[index_edge]);
        break;

    case RoleEdge::Bottom:
        _mat_model.rotate(angle, _edge_vectors_bottom[index_edge]);
        break;
    default:
        break;
    }

    _current_role_angle += angle;
}
