#include "commands.h"
#include "launchermain.h"

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
