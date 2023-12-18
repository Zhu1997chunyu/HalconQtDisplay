#include "HalconWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HalconWidget w;
    w.showMaximized();
    return a.exec();
}
