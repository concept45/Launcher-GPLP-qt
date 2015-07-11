#ifndef LAUNCHERMAIN_H
#define LAUNCHERMAIN_H

#include <QMainWindow>

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

private:
    Ui::LauncherMain *ui;

    void SetupFunctions();
};

#endif // LAUNCHERMAIN_H
