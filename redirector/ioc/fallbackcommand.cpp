#include <fallbackcommand.h>
#include <stdexcept>

FallbackCommand::FallbackCommand(const std::vector<ICommandPtr> &commands)
    :m_Commands(commands)
{
}


void FallbackCommand::execute()
{
    auto command = m_Commands.begin();
    while (command != m_Commands.end())
    {
        try
        {
            (*command)->execute();
            return;
        }
        catch (std::exception exception)
        {

        }
        command++;
    }

    throw std::runtime_error("FallbackCommand::execute(): all commands fails");
}
