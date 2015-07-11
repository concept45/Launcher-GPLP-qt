#include "launchermain.h"
#include "ui_launchermain.h"
#include <QMessageBox>

LauncherMain::LauncherMain(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    SetupFunctions();
}

LauncherMain::~LauncherMain()
{
    delete ui;
}

void LauncherMain::SetupFunctions()
{
    connect(ui->closeButton, &QPushButton::clicked, [=]()
    {
        if (!close())
            QMessageBox::information(nullptr, "Error while closing the ui", "Error");
    });
}
