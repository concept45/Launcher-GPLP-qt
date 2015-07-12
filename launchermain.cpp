#include "launchermain.h"
#include "ui_launchermain.h"
#include <QMessageBox>
#include <QLayout>

LauncherMain::LauncherMain(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain), _mouseClickXCoordinate(0), _mouseClickYCoordinate(0),
    _maximized(false), _init(false), _canMove(true), _grip(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    ui->comboBox->setCurrentText(QString());
    SetupFunctions();

    _grip = new QSizeGrip(ui->centralWidget);
    _grip->move(width() - 12, height() - 12);
}

LauncherMain::~LauncherMain()
{
    delete _grip;
    delete ui;
}

void LauncherMain::SetupFunctions()
{
    ui->closeButton->installEventFilter(this);
    ui->maximizeButton->installEventFilter(this);
    ui->minimizeButton->installEventFilter(this);

    connect(ui->closeButton, &QPushButton::clicked, [=]()
    {
        if (!close())
            QMessageBox::information(nullptr, "Error", "Error while closing the ui");
        qApp->exit();
    });

    connect(ui->closeButton, &QPushButton::pressed, [=]()
    {
        _canMove = false;
    });

    connect(ui->maximizeButton, &QPushButton::clicked, [=]()
    {
        if (!_maximized)
            showMaximized();
        else
            showNormal();

        _maximized = !_maximized;
    });

    connect(ui->minimizeButton, &QPushButton::clicked, [=]()
    {
        showMinimized();
    });

    //_init = true;
}

bool LauncherMain::IsFormButton(QObject* object) // Funcion de ayuda
{
    return object == ui->closeButton || object == ui->maximizeButton || object == ui->minimizeButton;
}

bool LauncherMain::eventFilter(QObject* object, QEvent* event)
{
    if (!_init)
        return false; // Si no se han iniciado la interfaz devolvemos false para no romper las proximas ejecuciones

    bool formButton = IsFormButton(object);

    if (formButton && event->type() == QEvent::KeyPress) // Ignoramos las teclas para el boton de cierre
        return true;

    if (formButton)
    {
        if (event->type() == QEvent::HoverEnter)
        {
            //ui->closeButton->setIcon(); Icono del boton en estado Hover
            return true;
        }
        else if (event->type() == QEvent::HoverLeave)
        {
            //ui->closeButton->setIcon(); Icono normal del boton.
            return true;
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            _canMove = true;
            return QObject::eventFilter(object, event); // Devolvemos el filter default para que funcionen los botones
        }
    }

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
    if (_maximized || !_init || !_canMove)
        return;

    int32 newX = event->globalX() - _mouseClickXCoordinate;
    int32 newY = event->globalY() - _mouseClickYCoordinate;
    move(newX, newY); // En un futuro con los paneles de la form web y demas esto solo se podra ejecutar en la parte superior
}

void RelocateButton(QPushButton* button, int32 newX)
{
    button->setGeometry(button->geometry().x() - newX, button->geometry().y(), button->geometry().width(), button->geometry().height());
}

void ModifyColumnView(QColumnView* column, bool width, int32 newParam)
{
    if (width)
        column->setGeometry(column->geometry().x(), column->geometry().y(), column->geometry().width() - newParam, column->geometry().height());
    else
        column->setGeometry(column->geometry().x(), column->geometry().y(), column->geometry().width(), column->geometry().height() - newParam);
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
    int32 currentY = event->oldSize().height();
    int32 newY = currentY - event->size().height();

    RelocateButton(ui->closeButton, newX);
    RelocateButton(ui->minimizeButton, newX);
    RelocateButton(ui->maximizeButton, newX);

    ModifyColumnView(ui->columnView, false, newY);
    ModifyColumnView(ui->columnView_2, true, newX);

    _grip->move(event->size().width() - 12, event->size().height() - 12);
}
