#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include <list>
#include "icommandqueue.h"
#include "exceptionmanager.h"

class CommandQueue : public ICommandQueue
{
    ExceptionManagerPtr m_exManager;
    std::list<ICommandPtr> m_commandList;
public:
    CommandQueue(ExceptionManagerPtr exManager, std::list<ICommandPtr> commandList = {});
    void push_back(ICommandPtr command) override;
    void push_front(ICommandPtr command) override;
    void next() override;
    size_t size() override;
};

#endif // COMMANDQUEUE_H
