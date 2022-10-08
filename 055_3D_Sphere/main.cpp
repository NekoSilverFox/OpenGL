#include "mainwindow.h"
#include <QApplication>

#if 0
    1. 对透视投影中的 Fov 和 ratio 参数进行试验。看能能否搞懂他们是如何影响平截头体的
    2. 将观察矩阵在各个方向上进行位移，看看场景是如何改变的。注意把观察矩阵当成摄像机对象
    3. 使用模型矩阵只让是3的倍数的箱子旋转（以及第一个箱子），而让剩下的箱子保持静止
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
