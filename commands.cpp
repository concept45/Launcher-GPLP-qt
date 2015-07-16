#include "commands.h"

template<bool (LauncherMain::*F)(const char*)>
bool CommandFunction(LauncherMain* launcher, const char* args)
{
    return (launcher->*F)(args);
}

ChatCommand* CommandParser::getCommandTable()
{
    static ChatCommand commandTable[] =
    {
        { "hello", CommandFunction<&LauncherMain::HandleHelloCommand>, "Comando de prueba", nullptr },
        { nullptr, nullptr,                                             "",                 nullptr },
    };

    return commandTable;
}

CommandParser::CommandParser() : _main(nullptr)
{
}

CommandParser::~CommandParser()
{
}

bool CommandParser::HasStringAbbr(char const* name, char const* part)
{
    if (*name)
    {
        if (!*part)
            return false;

        for (;;)
        {
            if (!*part)
                return true;
            else if (!*name)
                return false;
            else if (tolower(*name) != tolower(*part))
                return false;
            ++name; ++part;
        }
    }

    return true;
}

bool CommandParser::TryParseAndExecute(char const* text)
{
    std::string fullcmd = text;

    if (strlen(text) < 2)
        return false;

    if ((text[0] == '.' && text[1] == '.') || (text[0] == '!' && text[1] == '!'))
        return false;

    if (text[0] == '!' || text[0] == '.')
        ++text;

    if (!TryExecute(getCommandTable(), text, fullcmd))
        return false;

    return true;
}

bool CommandParser::TryExecute(ChatCommand* table, char const* text, std::string const& fullcmd)
{
    char const* oldtext = text;
    std::string cmd = "";

    while (*text != ' ' && *text != '\0')
    {
        cmd += *text;
        ++text;
    }

    while (*text == ' ') ++text;

    for (uint32 i = 0; table[i].Name != nullptr; ++i)
    {
        if (!HasStringAbbr(table[i].Name, cmd.c_str()))
            continue;

        bool match = false;
        if (strlen(table[i].Name) > cmd.length())
        {
            for (uint32 j = 0; table[j].Name != nullptr; ++j)
            {
                if (!HasStringAbbr(table[j].Name, cmd.c_str()))
                    continue;

                if (strcmp(table[j].Name, cmd.c_str()) != 0)
                    continue;
                else
                {
                    match = true;
                    break;
                }
            }
        }
        if (match)
            continue;

        if (table[i].ChildCommands != nullptr)
        {
            if (!TryExecute(table[i].ChildCommands, text, fullcmd))
            {
                /*if (text && text[0] != '\0')
                    "No existe ese subcomando."
                else
                    "Sintaxis incorrecta."

                ShowHelpForCommand(table[i].ChildCommands, text);*/
            }

            return true;
        }

        if (!table[i].Handler)
            continue;

        //SetSentErrorMessage(false);
        // table[i].Name == "" is special case: send original command to handler
        if (!((table[i].Handler)(_main, table[i].Name[0] != '\0' ? text : oldtext)) /*&& HasSentErrorMessage()*/)
        {
            //if (!table[i].Help.empty())
            //    (table[i].Help.c_str());
            //else
            //    "Sintaxis incorrecta.";
        }

        return true;
    }

    return false;
}
