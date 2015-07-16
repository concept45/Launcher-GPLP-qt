#include "commands.h"
#include "launchermain.h"

bool LauncherMain::HandleHelloCommand(char const* /*args*/)
{
    GetDevPanel()->GetEditor()->append("Hello");
    return true;
}
