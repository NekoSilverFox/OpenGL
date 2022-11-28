#ifndef FOXMATH_H
#define FOXMATH_H

#include <QMatrix4x4>
#include <QVector3D>

namespace FoxMath
{
/**
 * @brief 计算并返回由 3 各构成平面的法向量
 * @param p1 第一个点
 * @param p2 第二个点
 * @param p3 第三个点
 * @return
 */
QVector3D getNormalVector(QVector3D p1, QVector3D p2, QVector3D p3)
{
    return   QVector3D::crossProduct(p1, p2)
           + QVector3D::crossProduct(p2, p3)
           + QVector3D::crossProduct(p3, p1);
}

//绕任意轴的旋转
QMatrix4x4 rotateArbitraryLine(QMatrix4x4 rmatrix, QVector3D v1, QVector3D v2, float angle)
{
    float theta = angle * M_PI / 180.0f;  // 转为弧度

    float a = v1.x();
    float b = v1.y();
    float c = v1.z();

    QVector3D p1 = v2 - v1;
    QVector3D p = p1.normalized();

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

}


#endif // FOXMATH_H
