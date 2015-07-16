#include "launchermain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LauncherMain w;
    if (a.arguments().contains("-gplp_dev"))
        w.SetDevMode(true, true);
    w.show();

    return a.exec();
}
