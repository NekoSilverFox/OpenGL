#include "mainwindow.h"
#include <QApplication>


#if 0
    1. 绘制两个紧挨的三角形
    2. 使用不同的VAO和VBO进行管理
    3. 右侧的三角形绘制为黄色
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
