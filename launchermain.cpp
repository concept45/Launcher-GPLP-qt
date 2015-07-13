#include "launchermain.h"
#include "ui_launchermain.h"
#include <QMessageBox>
#include <QLayout>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
#include <sstream>

LauncherMain::LauncherMain(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::LauncherMain), _mouseClickXCoordinate(0), _mouseClickYCoordinate(0),
    _maximized(false), _init(false), _canMove(false), _grip(nullptr)
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
    SaveRealmlists();
    _dbEngine.close();
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
            QMessageBox::information(nullptr, "Error", "Error while closing the ui.");
        qApp->exit();
    });

    connect(ui->maximizeButton, &QPushButton::clicked, [=]()
    {
        if (!_maximized)
            showMaximized();
        else
            showNormal();

        _grip->setVisible(_maximized);
        _maximized = !_maximized;
    });

    connect(ui->minimizeButton, &QPushButton::clicked, [=]()
    {
        showMinimized();
    });

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ProcessQComboxSignal(int)));
}

void LauncherMain::InitDB()
{
    QString dbPath = QCoreApplication::applicationDirPath() + "/user.db";
    _dbEngine = QSqlDatabase::addDatabase("QSQLITE");
    _dbEngine.setDatabaseName(dbPath);
    QFileInfo checkFile(dbPath);
    if (checkFile.exists() && checkFile.isFile())
    {
        if (!_dbEngine.open())
            QMessageBox::information(nullptr, "DBError", _dbEngine.lastError().text());
        else
        {
            LoadRealmlists();
            // Añadir mas cosas
        }
    }
    else
    {
        QFile newFile(dbPath);
        newFile.open(QIODevice::ReadWrite | QIODevice::Text);
        newFile.close();
        if (_dbEngine.open())
        {
            QSqlQuery query;
            query.exec("CREATE TABLE `realmlists` (`realmName` LONGTEXT, `realmlist` LONGTEXT);");
        }
        else
            QMessageBox::information(nullptr, "DBError", _dbEngine.lastError().text());
    }
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
    }

    return QObject::eventFilter(object, event);
}

void LauncherMain::mousePressEvent(QMouseEvent* event)
{
    if (!_init)
        return;

    _mouseClickXCoordinate = event->x();
    _mouseClickYCoordinate = event->y();

    if (_maximized)
        return;

    if (event->type() == QMouseEvent::MouseButtonPress && event->button() == Qt::LeftButton)
        _canMove = true;
}

void LauncherMain::mouseReleaseEvent(QMouseEvent* event)
{
    if (_maximized || !_init)
        return;

    if (event->type() == QMouseEvent::MouseButtonRelease && event->button() == Qt::LeftButton)
        _canMove = false;
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

void LauncherMain::ProcessQComboxSignal(int index)
{
    if (index == 5)
    {
        if (!close())
            QMessageBox::information(nullptr, "Error", "Error while closing the ui");
        qApp->exit();
    }

    ui->comboBox->setCurrentText(QString());
}

void LauncherMain::LoadRealmlists()
{
    if (!_dbEngine.isOpen())
        return;

    _realmlistStore.clear();

    QSqlQuery query;
    query.exec("SELECT realmName, realmlist FROM realmlists;");
    while (query.next())
        _realmlistStore.insert(query.value(0).toString(), query.value(1).toString());
}

void LauncherMain::SaveRealmlists()
{
    if (!_dbEngine.isOpen())
        return;

    if (_realmlistStore.empty())
        return;

    QSqlQuery query;
    query.exec("DELETE FROM realmlists");
    std::ostringstream sqlSave;
    sqlSave << "INSERT INTO realmlists VALUES\n";
    bool first = false;
    for (auto itr = _realmlistStore.begin(); itr != _realmlistStore.end(); ++itr)
    {
        if (!first)
        {
            first = true;
            sqlSave << "('";
        }
        else
            sqlSave << ",\n('";


        sqlSave << itr.key().toStdString().c_str() << "'" << ", " <<
                   "'" << itr.value().toStdString().c_str() << "'" << ")";
    }

    sqlSave << ";";
    query.exec(sqlSave.str().c_str());
}
