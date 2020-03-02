#include "qpopwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPopWindow w;
    w.hide();

    return a.exec();
}
