#ifndef FOXMATH_H
#define FOXMATH_H

# include <QVector3D>

/**
 * @brief 计算并返回由 3 各构成平面的法向量
 * @param p1 第一个点
 * @param p2 第二个点
 * @param p3 第三个点
 * @return
 */
inline QVector3D getNormalVector(QVector3D p1, QVector3D p2, QVector3D p3)
{
    return   QVector3D::crossProduct(p1, p2)
           + QVector3D::crossProduct(p2, p3)
           + QVector3D::crossProduct(p3, p1);
}

#endif // FOXMATH_H
