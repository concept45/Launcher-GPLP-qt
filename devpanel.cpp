#include "devpanel.h"
#include "ui_devpanel.h"
#include <QScrollBar>

DevPanel::DevPanel(QWidget* parent) : QDialog(parent), ui(new Ui::DevPanel)
{
    _commandParser = new CommandParser(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
}

DevPanel::~DevPanel()
{
    delete _commandParser;
    delete ui;
}

bool DevPanel::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key()== Qt::Key_Return) // Segun la tabla de info....
        {
            QString command = ui->lineEdit->text();
            ui->lineEdit->setText("");
            TryExecuteCommand(command);
        }

        return QObject::eventFilter(object, event);
    }

    return QObject::eventFilter(object, event);
}

void DevPanel::SetText(QString text)
{
    ui->textEdit->setText(text);
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}

void DevPanel::AppendText(QString text)
{
    ui->textEdit->setText(ui->textEdit->toPlainText() + text + "\n");
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}

void DevPanel::TryExecuteCommand(QString text)
{
    CommandResponse response = _commandParser->TryParseAndExecute(text.toStdString().c_str());
    if (response == COMMAND_RESPONSE_HELP_DATA)
        _commandParser->ShowHelpForCommand(_commandParser->getCommandTable(), text.toStdString().c_str());
    else
    {
        std::string responseStr = _commandParser->GetResponseString(response);
        if (responseStr != "")
            AppendText(QString(responseStr.c_str()));
    }
}
