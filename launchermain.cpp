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
    ui->closeButton->installEventFilter(this)
    connect(ui->closeButton, &QPushButton::clicked, [=]()
    {
        if (!close())
            QMessageBox::information(nullptr, "Error while closing the ui", "Error");
    });
}

bool LauncherMain::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui->closeButton && event->type() == QEvent::KeyPress) // Ignoramos las teclas para el boton de cierre
        return true;
    else
        return QObject::eventFilter(object, event);
}
