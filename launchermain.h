#ifndef LAUNCHERMAIN_H
#define LAUNCHERMAIN_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QSizeGrip>
#include <QSql>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QHash>
#include "types.h"

namespace Ui
{
    class LauncherMain;
}

class LauncherMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit LauncherMain(QWidget* parent = 0);
    ~LauncherMain();

    bool eventFilter(QObject* object, QEvent* event);

    bool IsFormButton(QObject* object);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void resizeEvent(QResizeEvent* event);

private:
    Ui::LauncherMain* ui;

    void SetupFunctions();
    void LoadRealmlists();
    void SaveRealmlists();

    int32 _mouseClickXCoordinate;
    int32 _mouseClickYCoordinate;

    bool _maximized;
    bool _init;
    bool _canMove;

    QSizeGrip* _grip;

    QSqlDatabase _dbEngine;

    QHash<QString, QString> _realmlistStore;

private slots:
    void ProcessQComboxSignal(int index);
};

#endif // LAUNCHERMAIN_H
