#include "mywidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // 新建QApplication对象，带接受入参
    QApplication a(argc, argv);
    // 新建一个MyWidget 窗体，具体窗体内容实现在MyWidget.h中
    MyWidget w;
    w.show();
    return a.exec();
}
