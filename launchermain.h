#ifndef LAUNCHERMAIN_H
#define LAUNCHERMAIN_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QResizeEvent>
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
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);

    void resizeEvent(QResizeEvent* event);

    bool IsFormButton(QObject* object);

private:
    Ui::LauncherMain *ui;

    void SetupFunctions();

    int32 _mouseClickXCoordinate;
    int32 _mouseClickYCoordinate;

    bool _maximized;
    bool _init;
};

#endif // LAUNCHERMAIN_H
