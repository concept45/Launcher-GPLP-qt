#include "launchermain.h"
#include "ui_launchermain.h"
#include <QMessageBox>

LauncherMain::LauncherMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);

    connect(ui->closeButton, &QPushButton::clicked, this, &LauncherMain::CloseMainWindows);
    //connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(CloseMainWindows()));
}

LauncherMain::~LauncherMain()
{
    delete ui;
}

void LauncherMain::CloseMainWindows()
{
    if (!close())
        QMessageBox::information(nullptr, "Error while closing the ui", "Error");
}
