/**********************************************************
Author: Qt君
微信公众号: Qt君(首发)
QQ群: 732271126
Email: 2088201923@qq.com
LICENSE: MIT
**********************************************************/
#include "QtCamera.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVideoWidget widget;
    QtCamera camera(&widget);
    widget.show();
    return a.exec();
}
