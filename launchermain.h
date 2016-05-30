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
#include "devpanel.h"

#ifndef G_ASSERT
    #define G_ASSERT(cond) ((!(cond)) ? qt_assert(#cond,__FILE__,__LINE__) : qt_noop())
#endif

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

    void SetDevMode(bool x, bool init);
    void SetDevPanel(DevPanel* dialog);
    DevPanel* GetDevPanel() { return _devPanel; }

private:
    Ui::LauncherMain* ui;

    void SetupFunctions();
    void InitDB();
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

    bool _devMode;

    DevPanel* _devPanel;

private slots:
    void ProcessQComboxSignal(int index);
};

#endif // LAUNCHERMAIN_H
