#include "Command.hpp"

Command::Command()
{
    mName = "";
    mHelp = "";
    mManual = "";
}

std::string Command::getName() const
{
    return mName;
}

std::string Command::getHelp() const
{
    return mHelp;
}

std::string Command::getManual() const
{
    return mManual;
}

void Command::execute(std::string options)
{
    if (mFunction)
    {
        mFunction(splitOptions(options));
    }
    else
    {
        std::cout << "Command not properly set" << std::endl;
    }
}

void Command::setName(std::string const& name)
{
    mName = name;
}

void Command::setHelp(std::string const& help)
{
    mHelp = help;
}

void Command::setManual(std::string const& manual)
{
    mManual = manual;
}

void Command::setFunction(Command::CommandFunction function)
{
    mFunction = function;
}

bool Command::hasAlias(std::string const& alias)
{
    for (auto a : mAlias)
    {
        if (a == alias)
        {
            return true;
        }
    }
    return false;
}

void Command::addAlias(std::string const& alias)
{
    mAlias.push_back(alias);
}

bool Command::isCommand(std::string const& name)
{
    return (name == mName || hasAlias(name));
}

Command::OptionSplit Command::splitOptions(std::string options)
{
    OptionSplit split;
    bool open = false;
    bool readOption = false;
    bool readArgs = false;
    std::size_t readPos = 0;
    std::string actualOption = "";
    std::string actualArg = "";
    for (std::size_t i = 0; i < options.size(); i++)
    {
        if (options[i] == '-' && !open)
        {
            readOption = true;
            readPos = i;
        }
        else if (options[i] == ' ' && readOption == true)
        {
            actualOption = options.substr(readPos+1,i-readPos-1);
            split[actualOption] = {};
            readOption = false;
            readArgs = true;
            readPos = i;
        }
        else if (options[i] == ' ' && readArgs && !open)
        {
            actualArg = options.substr(readPos+1,i-readPos-1);
            if (actualArg != "")
            {
                if (actualArg[0] == '\"' && actualArg[actualArg.size()-1] == '\"')
                {
                    actualArg = actualArg.substr(1,actualArg.size()-2);
                }
                split[actualOption].push_back(actualArg);
            }
            readPos = i;
        }
        else if (options[i] == '\"' && readArgs && !open)
        {
            open = true;
        }
        else if (options[i] == '\"' && readArgs && open)
        {
            open = false;
        }
        else if (options[i] == '-' && readArgs && !open)
        {
            readArgs = false;
        }
    }
    if (readOption)
    {
        actualOption = options.substr(readPos+1);
        split[actualOption] = {};
        readOption = false;
        readArgs = false;
    }
    else if (readArgs)
    {
        actualArg = options.substr(readPos+1);
        if (actualArg != "")
        {
            if (actualArg[0] == '\"' && actualArg[actualArg.size()-1] == '\"')
            {
                actualArg = actualArg.substr(1,actualArg.size()-2);
            }
            split[actualOption].push_back(actualArg);
        }
    }
    return split;
}
