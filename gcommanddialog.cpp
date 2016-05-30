#include "gcommanddialog.h"
#include "commands.h"

GCommandDialog::GCommandDialog(LauncherMain* base, QWidget* parent) :
    QDialog(parent), _base(base), _textEdit(nullptr)
{
}

GCommandDialog::~GCommandDialog()
{
    delete _textEdit;
}

void GCommandDialog::closeEvent(QCloseEvent* event)
{
    if (_base)
        _base->SetDevPanel(nullptr);
    event->accept();
    deleteLater();
}

void GCommandDialog::SetTextEdit(QTextEdit* textEdit)
{
    G_ASSERT(textEdit);
    _textEdit = textEdit;
    G_ASSERT(_textEdit);
    _textEdit->installEventFilter(this);
}

bool GCommandDialog::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key()== Qt::Key_Return) // Segun la tabla de info....
        {
            QStringList _textLines = _textEdit->toPlainText().split("\n");
            QString command = _textLines.last();

            if (CommandResponse response = sCommandParser->TryParseAndExecute(command.toStdString().c_str()))
            {
                if (response == COMMAND_RESPONSE_HELP_DATA)
                {
                    sCommandParser->ShowHelpForCommand(sCommandParser->getCommandTable(), command.toStdString().c_str());
                }
                else
                {
                    std::string responseStr = sCommandParser->GetResponseString(response);
                    if (responseStr != "")
                        _textEdit->append(responseStr.c_str());
                }
            }
        }

        return QObject::eventFilter(object, event);
    }

    return QObject::eventFilter(object, event);
}

