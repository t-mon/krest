#include "core.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Core::instance();
    
    int ret = a.exec();

    delete Core::instance();

    return ret;
}
