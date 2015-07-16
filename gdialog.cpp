#include "gdialog.h"
#include "commands.h"

GDialog::GDialog(LauncherMain* base, QWidget* parent) :
    QDialog(parent), _base(base), _textEdit(nullptr)
{
}

GDialog::~GDialog()
{
    delete _textEdit;
}

void GDialog::closeEvent(QCloseEvent* event)
{
    if (_base)
        _base->SetDevPanel(nullptr);
    event->accept();
    deleteLater();
}

void GDialog::SetTextEdit(QTextEdit* textEdit)
{
    Q_ASSERT(textEdit);
    _textEdit = textEdit;
    Q_ASSERT(_textEdit);
    _textEdit->installEventFilter(this);
}

bool GDialog::eventFilter(QObject* object, QEvent* event)
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

