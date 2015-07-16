#ifndef COMMANDS_H
#define COMMANDS_H

#include "launchermain.h"
#include <QVector>

class LauncherMain;

class ChatCommand
{
public:
    const char *       Name;
    bool (*Handler)(LauncherMain*, const char* args);
    std::string        Help;
    ChatCommand*      ChildCommands;
};

class CommandParser
{
public:
    explicit CommandParser();
    ~CommandParser();

    static CommandParser* instance()
    {
        static CommandParser instance;
        return &instance;
    }

    void SetLauncherMain(LauncherMain* launcher)
    {
        Q_ASSERT(launcher);
        _main = launcher;
        Q_ASSERT(_main);
    }

    bool TryParseAndExecute(char const* text);
    bool TryExecute(ChatCommand* table, const char* text, const std::string& fullcmd);
    bool HasStringAbbr(char const* name, char const* part);

    ChatCommand* getCommandTable();

private:
    LauncherMain* _main;
};

#define sCommandParser CommandParser::instance()

#endif // COMMANDS_H

