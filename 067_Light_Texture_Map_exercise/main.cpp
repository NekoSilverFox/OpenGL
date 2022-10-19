#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

#if 0

在片段着色器中反转镜面光贴图的颜色值，让木头显示镜面高光而钢制边缘不反光：参考解答
    反转颜色只需要将 颜色减去 (1, 1, 1) 即可

使用漫反射贴图创建一个彩色而不是黑白的镜面光贴图。可以使用这张彩色的镜面光贴图：最终效果

添加一个叫做放射光贴图(Emission Map)的东西，它是一个储存了每个片段的发光值(Emission Value)的贴图。将这个纹理（作者为 creativesam）作为放射光贴图添加到箱子上，产生这些字母都在发光的效果：参考解答，最终效果


#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);


    MainWindow w;
    w.show();

    return a.exec();
}
