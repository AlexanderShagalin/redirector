#ifndef ICOMMANDQUEUE_H
#define ICOMMANDQUEUE_H

#include "icommand.h"

class ICommandQueue
{
public:
    virtual ~ICommandQueue(){}
    virtual void push_back(ICommandPtr command) = 0;
    virtual void push_front(ICommandPtr command) = 0;
    virtual void next() = 0;
    virtual size_t size() = 0;
};

typedef std::shared_ptr<ICommandQueue> ICommandQueuePtr;
#endif // ICOMMANDQUEUE_H
