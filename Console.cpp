#include "Console.hpp"

Console::Console()
{
    mRunning = false;
    mUnknownCommandMessage = "Unknown command";

    Command quit;
    quit.setName("quit");
    quit.setHelp("Quit the console");
    quit.setManual("quit\nQuit the console properly\nNo options available");
    quit.setFunction([this](Command::OptionSplit options){stop();});
    addCommand(quit);

    Command help;
    help.setName("help");
    help.setHelp("See the list of commands with short description");
    help.setManual("help\nSee the list of commands with short description\nNo options available");
    help.setFunction([this](Command::OptionSplit options)
    {
        for (auto command : mCommands)
        {
            std::cout << command.getName() << " : " << command.getHelp() << std::endl;
        }
    });
    addCommand(help);

    Command man;
    man.setName("man");
    man.setHelp("See detailed information for a command");
    man.setManual("man\nSee detailed information for a command\nOptions : -c commandName");
    man.setFunction([this](Command::OptionSplit options)
    {
        if (options["c"].size() == 1)
        {
            std::string commandName = options["c"][0];
            for (auto command : mCommands)
            {
                if (command.isCommand(commandName))
                {
                    std::cout << command.getManual() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "Error with the options, ex : man -c man" << std::endl;
        }
    });
    addCommand(man);
}

void Console::addCommand(Command const& command)
{
    mCommands.push_back(command);
}

void Console::removeCommand(std::string const& command)
{
    for (std::size_t i = 0; i < mCommands.size();)
    {
        if (mCommands[i].isCommand(command))
        {
            mCommands.erase(i + mCommands.begin());
        }
        else
        {
            i++;
        }
    }
}

void Console::run()
{
    mRunning = true;
    while (mRunning)
    {
        std::cout << "-> ";
        std::string cmd;
        std::getline(std::cin,cmd);

        std::string commandName = getCommandName(cmd);
        std::string commandOptions = getCommandOptions(cmd);

        bool found = false;
        for (std::size_t i = 0; i < mCommands.size(); i++)
        {
            if (mCommands[i].isCommand(commandName))
            {
                found = true;
                mCommands[i].execute(commandOptions);
            }
        }
        if (!found)
        {
            std::cout << mUnknownCommandMessage << std::endl;
        }

        for (auto task :mTasks)
        {
            if (task)
            {
                task();
            }
        }

        if (mRunning)
        {
            std::cout << std::endl;
        }
    }
}

void Console::stop()
{
    mRunning = false;
}

void Console::addTask(std::function<void()> task)
{
    mTasks.push_back(task);
}

std::string Console::getCommandName(std::string const& input)
{
    std::string commandName;
    std::size_t found = input.find_first_of(" ");
    if (found != std::string::npos)
    {
        commandName = input.substr(0,found);
    }
    else
    {
        commandName = input;
    }
    return commandName;
}

std::string Console::getCommandOptions(std::string const& input)
{
    std::string commandOptions = "";
    std::size_t found = input.find_first_of(" ");
    if (found != std::string::npos)
    {
        commandOptions = input.substr(found+1);
    }
    return commandOptions;
}
