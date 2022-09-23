#include "mainwindow.h"
#include <QApplication>

#if 0

1. 修改片段着色器，使雪狸图案朝另一个方向看
2. 尝试不同的纹理环绕方式，设定一个从 0.0f 到 2.0f 范围内的（而不是原来的0.0f 到 1.0f）纹理坐标。试试看能不能放置4个
3. 尝试在矩形上只显示纹理图像中间的第一部分，修改纹理坐标，达到刚好能看见单个像素的效果。尝试使用 GL_NEAREST 的纹理过滤方式让像素现实的更加清晰
4. 使用一个 uniform变量作为mix函数的第三个参数来改变两个纹理的可见度，使用上下键来改变箱子或者笑脸的可见度

#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
