
#include <stdexcept>

#include "macrocommand.h"
 
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
            throw e;
            return;
        }
    }
}
