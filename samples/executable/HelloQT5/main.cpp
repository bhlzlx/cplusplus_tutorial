#include "HelloQT5.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HelloQT5 w;
    w.show();
    return a.exec();
}
