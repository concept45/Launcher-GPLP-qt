#include "commandparser.h"
#include "launchermain.h"
#include "downloadhandler.h"
#include <QApplication>

bool DevPanel::HandleHelloCommand(char const* /*args*/)
{
    AppendText("Hello <3");
    return true;
}

bool DevPanel::HandleHelloWorldCommand(char const* /*args*/)
{
    AppendText("Hello world!");
    return true;
}

bool DevPanel::HandleHelpCommand(char const* args)
{
    char const* cmd = strtok((char*)args, " ");
    if (!cmd)
    {
        _commandParser->ShowHelpForCommand(_commandParser->getCommandTable(), "help");
        _commandParser->ShowHelpForCommand(_commandParser->getCommandTable(), "");
    }
    else
    {
        if (!_commandParser->ShowHelpForCommand(_commandParser->getCommandTable(), cmd))
            AppendText("No hay ayuda disponible para ese comando.");
    }

    return true;
}

bool DevPanel::HandleClearConsoleCommand(char const* /*args*/)
{
    SetText("");
    return true;
}

bool DevPanel::HandleExitCommand(char const* /*args*/)
{
    qApp->exit();
    return true;
}

bool DevPanel::HandleDownloadTestCommand(char const* args)
{
    if (!*args)
        return false;

    QUrl url;
    url.setUrl(args);
    DownloadHandler* downloader = new DownloadHandler(this);
    downloader->RequestDownload(url);
    return true;
}
