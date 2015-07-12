#include "launchermain.h"
#include "ui_launchermain.h"
#include <QMessageBox>
#include <QLayout>

LauncherMain::LauncherMain(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain), _mouseClickXCoordinate(0), _mouseClickYCoordinate(0),
    _maximized(false), _init(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    //ui->statusBar->setSizeGripEnabled(false); // Temporal, see issue #3
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
            QMessageBox::information(nullptr, "Error", "Error while closing the ui");
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

    //_init = true;
}

bool LauncherMain::eventFilter(QObject* object, QEvent* event)
{
    if (!_init)
        return true;

    if (object == ui->closeButton && event->type() == QEvent::KeyPress) // Ignoramos las teclas para el boton de cierre
        return true;
    else
        return QObject::eventFilter(object, event);
}

void LauncherMain::mousePressEvent(QMouseEvent* event)
{
    if (_maximized || !_init)
        return;

    _mouseClickXCoordinate = event->x();
    _mouseClickYCoordinate = event->y();
}

void LauncherMain::mouseMoveEvent(QMouseEvent* event)
{
    if (_maximized || !_init)
        return;

    int32 newX = event->globalX() - _mouseClickXCoordinate;
    int32 newY = event->globalY() - _mouseClickYCoordinate;
    move(newX, newY); // En un futuro con los paneles de la form web y demas esto solo se podra ejecutar en la parte superior
}

void RelocateButton(QPushButton* button, int32 newX) // Funcion de ayuda
{
    button->setGeometry(button->geometry().x() - newX, button->geometry().y(), button->geometry().width(), button->geometry().height());
}

void LauncherMain::resizeEvent(QResizeEvent* event)
{
    if (!_init)
    {
        _init = true; // Solo tendremos en cuenta los eventos que pasen a partir del primer resize que es cuando se muestra la form
        return;
    }

    int32 currentX = event->oldSize().width();
    int32 newX = currentX - event->size().width();

    RelocateButton(ui->closeButton, newX);
    RelocateButton(ui->minimizeButton, newX);
    RelocateButton(ui->maximizeButton, newX);
}
