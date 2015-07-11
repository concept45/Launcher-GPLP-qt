#ifndef LAUNCHERMAIN_H
#define LAUNCHERMAIN_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class LauncherMain;
}

class LauncherMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit LauncherMain(QWidget *parent = 0);
    ~LauncherMain();

    bool eventFilter(QObject* object, QEvent* event);
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);

private:
    Ui::LauncherMain *ui;

    void SetupFunctions();

    int  _mouseClickXCoordinate;
    int  _mouseClickYCoordinate;

    bool _maximized;
};

#endif // LAUNCHERMAIN_H
