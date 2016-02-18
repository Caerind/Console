#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Command.hpp"

class Console
{
    public:
        Console();

        void addCommand(Command const& command);
        void removeCommand(std::string const& command);

        void run();
        void stop();

        void addTask(std::function<void()> task);

        static std::string getCommandName(std::string const& input);
        static std::string getCommandOptions(std::string const& input);

    private:
        bool mRunning;
        std::string mUnknownCommandMessage;
        std::vector<Command> mCommands;
        std::vector<std::function<void()>> mTasks;
};

#endif // CONSOLE_HPP
