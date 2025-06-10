#include "stdio.h"
#include "commandqueue.h"

CommandQueue::CommandQueue(ExceptionManagerPtr exManager, std::list<ICommandPtr> commandList) 
{ }

void CommandQueue::next()
{
    if(m_commandList.size() <= 0)
        return;

    auto command = m_commandList.front();
    m_commandList.pop_front();

    try
    {
        command->execute();
    }
    catch(std::exception& e)
    {
        if(m_exManager)
            m_exManager->handle(e, command);
        else
            throw e;
    }
}

void CommandQueue::push_back(ICommandPtr command)
{
    m_commandList.push_back(command);
}

void CommandQueue::push_front(ICommandPtr command)
{
    m_commandList.push_front(command);
}
    
size_t CommandQueue::size()
{
    return m_commandList.size();
}
