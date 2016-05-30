#ifndef GCOMMANDDIALOG_H
#define GCOMMANDDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include "launchermain.h"

class LauncherMain;

class GCommandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GCommandDialog(LauncherMain* base, QWidget* parent = 0);
    ~GCommandDialog();

    void closeEvent(QCloseEvent* event);

    void SetTextEdit(QTextEdit* textEdit);
    QTextEdit* GetEditor() { return _textEdit; }

    bool eventFilter(QObject* object, QEvent* event);

private:
    LauncherMain* _base;
    QTextEdit* _textEdit;
};

#endif // GCOMMANDDIALOG_H

