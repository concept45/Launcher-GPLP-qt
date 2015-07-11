#include "launchermain.h"
#include "ui_launchermain.h"

LauncherMain::LauncherMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain)
{
    ui->setupUi(this);
}

LauncherMain::~LauncherMain()
{
    delete ui;
}
