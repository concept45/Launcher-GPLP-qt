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

private:
    Ui::LauncherMain *ui;

private slots:
    void CloseMainWindows();
};

#endif // LAUNCHERMAIN_H
