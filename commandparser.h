#ifndef COMMANDS_H
#define COMMANDS_H

#include "devpanel.h"
#include <QVector>

class DevPanel;

enum CommandResponse
{
    COMMAND_RESPONSE_DONT_EXISTS    = 0,
    COMMAND_RESPONSE_DONT_EXISTS_S  = 1,
    COMMAND_RESPONSE_SINTAXIS_ERROR = 2,
    COMMAND_RESPONSE_DONT_REPORT    = 3,
    COMMAND_RESPONSE_HELP_DATA      = 4,
    COMMAND_RESPONSE_OK             = 5,
};

class ChatCommand
{
public:
    const char *       Name;
    bool (*Handler)(DevPanel*, const char* args);
    std::string        Help;
    ChatCommand*      ChildCommands;
};

class CommandParser
{
public:
    explicit CommandParser(DevPanel* parent);
    ~CommandParser();

    CommandResponse TryParseAndExecute(char const* text);
    CommandResponse TryExecute(ChatCommand* table, const char* text, const std::string& fullcmd);
    bool HasStringAbbr(char const* name, char const* part);
    bool ShowHelpForCommand(ChatCommand* table, const char* cmd);
    bool ShowHelpForSubCommands(ChatCommand* table, char const* cmd, char const* subcmd);

    char const* GetResponseString(CommandResponse response);

    ChatCommand* getCommandTable();

private:
    DevPanel* _form;
};

#endif // COMMANDS_H
