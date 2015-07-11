#include "launchermain.h"
#include "ui_launchermain.h"
#include <QMessageBox>
#include <QLayout>

LauncherMain::LauncherMain(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain), _mouseClickXCoordinate(0), _mouseClickYCoordinate(0),
    _maximized(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    ui->statusBar->setSizeGripEnabled(false); // Temporal, see issue #3
    SetupFunctions();
}

LauncherMain::~LauncherMain()
{
    delete ui;
}

void LauncherMain::SetupFunctions()
{
    ui->closeButton->installEventFilter(this);

    connect(ui->closeButton, &QPushButton::clicked, [=]()
    {
        if (!close())
            QMessageBox::information(nullptr, "Error while closing the ui", "Error");
    });

    connect(ui->maximizeButton, &QPushButton::clicked, [=]()
    {
        // Temporal, see issue #3
        /*if (!_maximized)
            showMaximized();
        else
            showNormal();

        _maximized = !_maximized;*/
    });

    connect(ui->minimizeButton, &QPushButton::clicked, [=]()
    {
        showMinimized();
    });
}

bool LauncherMain::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui->closeButton && event->type() == QEvent::KeyPress) // Ignoramos las teclas para el boton de cierre
        return true;
    else
        return QObject::eventFilter(object, event);
}

void LauncherMain::mousePressEvent(QMouseEvent* event)
{
    if (_maximized)
        return;

    _mouseClickXCoordinate = event->x();
    _mouseClickYCoordinate = event->y();
}

void LauncherMain::mouseMoveEvent(QMouseEvent* event)
{
    if (_maximized)
        return;

    int newX = event->globalX() - _mouseClickXCoordinate;
    int newY = event->globalY() - _mouseClickYCoordinate;
    move(newX, newY); //En un futuro con los paneles de la form web y demas esto solo se podra ejecutar en la parte superior
}
