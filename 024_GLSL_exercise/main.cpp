#include "mainwindow.h"
#include <QApplication>

#if 0

1. 修改顶点着色器让三角形上下颠倒
2. 使用 uniform 定义一个水平偏移量，在顶点着色器中使用这个偏移量将三角形移动到屏幕右侧

#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
