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
        { "help",  CommandFunction<&LauncherMain::HandleHelpCommand>,  "",                   nullptr },
        { "hello", CommandFunction<&LauncherMain::HandleHelloCommand>, "Comando de prueba.", nullptr },
        { "cls",   CommandFunction<&LauncherMain::HandleClearConsoleCommand>, "",            nullptr },
        { nullptr, nullptr,                                            "",                   nullptr },
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

CommandResponse CommandParser::TryParseAndExecute(char const* text)
{
    std::string fullcmd = text;

    if (strlen(text) < 2)
        return COMMAND_RESPONSE_DONT_REPORT;

    if ((text[0] == '.' && text[1] == '.') || (text[0] == '!' && text[1] == '!'))
        return COMMAND_RESPONSE_DONT_REPORT;

    if (text[0] == '!' || text[0] == '.')
        ++text;

    if (CommandResponse response = TryExecute(getCommandTable(), text, fullcmd))
        return response;

    return COMMAND_RESPONSE_OK;
}

CommandResponse CommandParser::TryExecute(ChatCommand* table, char const* text, std::string const& fullcmd)
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
                ShowHelpForCommand(table[i].ChildCommands, text);
                if (text && text[0] != '\0')
                    return COMMAND_RESPONSE_DONT_EXISTS_S;
                else
                    return COMMAND_RESPONSE_SINTAXIS_ERROR;
            }

            return COMMAND_RESPONSE_OK;
        }

        if (!table[i].Handler)
            continue;

        if (!((table[i].Handler)(_main, table[i].Name[0] != '\0' ? text : oldtext)))
        {
            if (!table[i].Help.empty())
                return COMMAND_RESPONSE_HELP_DATA;
            else
                return COMMAND_RESPONSE_SINTAXIS_ERROR;
        }

        return COMMAND_RESPONSE_OK;
    }

    return COMMAND_RESPONSE_DONT_EXISTS;
}

bool CommandParser::ShowHelpForCommand(ChatCommand* table, const char* cmd)
{
    if (*cmd)
    {
        for (uint32 i = 0; table[i].Name != nullptr; ++i)
        {
            if (!HasStringAbbr(table[i].Name, cmd))
                continue;

            char const* subcmd = (*cmd) ? strtok(nullptr, " ") : "";

            if (table[i].ChildCommands && subcmd && *subcmd)
            {
                if (ShowHelpForCommand(table[i].ChildCommands, subcmd))
                    return true;
            }

            if (!table[i].Help.empty())
                _main->GetDevPanel()->GetEditor()->append(table[i].Help.c_str());

            if (table[i].ChildCommands)
                if (ShowHelpForSubCommands(table[i].ChildCommands, table[i].Name, subcmd ? subcmd : ""))
                    return true;

            return !table[i].Help.empty();
        }
    }
    else
    {
        for (uint32 i = 0; table[i].Name != nullptr; ++i)
        {
            if (strlen(table[i].Name))
                continue;

            if (!table[i].Help.empty())
                _main->GetDevPanel()->GetEditor()->append(table[i].Help.c_str());

            if (table[i].ChildCommands)
                if (ShowHelpForSubCommands(table[i].ChildCommands, "", ""))
                    return true;

            return !table[i].Help.empty();
        }
    }

    return ShowHelpForSubCommands(table, "", cmd);
}

bool CommandParser::ShowHelpForSubCommands(ChatCommand* table, char const* cmd, char const* subcmd)
{
    std::string list;
    bool first = true;
    for (uint32 i = 0; table[i].Name != nullptr; ++i)
    {
        if (*subcmd && !HasStringAbbr(table[i].Name, subcmd))
            continue;

        if (!first)
            list += "\n    ";
        else
        {
            first = false;
            list += "    ";
        }

        list += table[i].Name;

        if (table[i].ChildCommands)
            list += " ...";
    }

    if (list.empty())
        return false;

    if (table == getCommandTable())
    {
        _main->GetDevPanel()->GetEditor()->append("Comandos disponibles:");
        QString outStr = QString::asprintf("%s", list.c_str());
        _main->GetDevPanel()->GetEditor()->append(outStr);
    }
    else
    {
        QString outStr = QString::asprintf("El comando %s tiene subcomandos: %s", cmd, list.c_str());
        _main->GetDevPanel()->GetEditor()->append(outStr);
    }

    return true;
}

char const* CommandParser::GetResponseString(CommandResponse response)
{
    switch (response)
    {
        case COMMAND_RESPONSE_DONT_EXISTS:
            return "No existe ese comando.";
        case COMMAND_RESPONSE_DONT_EXISTS_S:
            return "No existe ese subcomando.";
        case COMMAND_RESPONSE_SINTAXIS_ERROR:
            return "Sintaxis incorrecta.";
        default:
            break;
    }

    return "";
}
