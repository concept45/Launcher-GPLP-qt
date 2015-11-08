#include "commands.h"
#include "launchermain.h"
#include "filedownloader.h"

bool LauncherMain::HandleHelloCommand(char const* /*args*/)
{
    GetDevPanel()->GetEditor()->append("Hello");
    return true;
}

bool LauncherMain::HandleHelpCommand(char const* args)
{
    char const* cmd = strtok((char*)args, " ");
    if (!cmd)
    {
        sCommandParser->ShowHelpForCommand(sCommandParser->getCommandTable(), "help");
        sCommandParser->ShowHelpForCommand(sCommandParser->getCommandTable(), "");
    }
    else
    {
        if (!sCommandParser->ShowHelpForCommand(sCommandParser->getCommandTable(), cmd))
            GetDevPanel()->GetEditor()->append("No hay ayuda disponible para ese comando.");
    }

    return true;
}

bool LauncherMain::HandleClearConsoleCommand(char const* /*args*/)
{
    GetDevPanel()->GetEditor()->setText("");
    return true;
}

bool LauncherMain::HandleDownloadTestCommand(char const* args)
{
    if (!*args)
        return false;

    QUrl url;
    url.setUrl(args);
    FileDownloader downloader(url);
    downloader.Execute();
    return true;
}