#ifndef GDIALOG_H
#define GDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include "launchermain.h"

class LauncherMain;

class GDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GDialog(LauncherMain* base, QWidget* parent = 0);
    ~GDialog();

    void closeEvent(QCloseEvent* event);

    void SetTextEdit(QTextEdit* textEdit);
    QTextEdit* GetEditor() { return _textEdit; }

    bool eventFilter(QObject* object, QEvent* event);

private:
    LauncherMain* _base;
    QTextEdit* _textEdit;
};

#endif // GDIALOG_H

