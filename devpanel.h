#ifndef DEVPANEL_H
#define DEVPANEL_H

#include <QDialog>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QSizeGrip>
#include <QSql>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QHash>
#include "commandparser.h"
#include "types.h"

class CommandParser;

namespace Ui
{
    class DevPanel;
}

class DevPanel : public QDialog
{
    Q_OBJECT

public:
    explicit DevPanel(QWidget* parent = 0);
    ~DevPanel();

    bool eventFilter(QObject* object, QEvent* event);

    void SetText(QString text);
    void AppendText(QString text);
    void TryExecuteCommand(QString text);

private:
    Ui::DevPanel* ui;
    CommandParser* _commandParser;

public: // Zona Comandos
   bool HandleHelloCommand(char const* args);
   bool HandleHelloWorldCommand(char const* args);
   bool HandleHelpCommand(char const* args);
   bool HandleClearConsoleCommand(char const* args);
   bool HandleExitCommand(char const* args);
   bool HandleDownloadTestCommand(char const* args);
};

#endif // DEVPANEL_H
