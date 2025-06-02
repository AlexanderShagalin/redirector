
#include <stdexcept>

#include "macrocommand.h"
#include "commandexception.h"
 
MacroCommand::MacroCommand(std::list<ICommandPtr> commands)
{
    m_commands = commands;
}

void MacroCommand::execute() 
{
    for(auto command : m_commands)
    {
        try
        {
            command->execute();
        }
        catch(const std::exception& e)
        {
            throw CommandException(e);
            return;
        }
    }
}
