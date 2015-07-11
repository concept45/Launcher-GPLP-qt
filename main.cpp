#include "launchermain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LauncherMain w;
    w.show();

    return a.exec();
}
